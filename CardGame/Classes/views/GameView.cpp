#include "GameView.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

bool GameView::init()
{
    if (!Layer::init()) return false;
    setContentSize(Size(1080, 2080));

    auto deskLayer = LayerColor::create(Color4B(34, 139, 34, 255), 1080, 1500);
    deskLayer->setPosition(0, 580);
    addChild(deskLayer, -2);

    auto handLayer = LayerColor::create(Color4B(139, 69, 19, 255), 1080, 580);
    handLayer->setPosition(0, 0);
    addChild(handLayer, -2);

    return true;
}

void GameView::renderPlayfield(const std::vector<CardModel*>& cards)
{
    for (auto it = _cardViews.begin(); it != _cardViews.end();)
    {
        if (std::find(cards.begin(), cards.end(), it->first) == cards.end())
        {
            it->second->removeFromParent();
            it = _cardViews.erase(it);
        }
        else ++it;
    }

    for (auto* cardModel : cards)
    {
        CardView* cardView = nullptr;
        if (_cardViews.count(cardModel) == 0)
        {
            cardView = CardView::create(cardModel);
            _cardViews[cardModel] = cardView;
            addChild(cardView);
        }
        else cardView = _cardViews[cardModel];

        
        cardView->refresh();
        cardView->setPosition((cardModel->getCardPos()));
        cardView->setScale(1.0f);
        cardView->setLocalZOrder(10);
    }
}

void GameView::renderStack(const std::vector<CardModel*>& cards)
{
    if (cards.empty()) return;

    const float CARD_WIDTH = 120.0f, MIN_SPACE = 6.0f, MAX_SPACE = 40.0f, TOP_GAP = 100.0f, BASE_Y = 200.0f;
    const float LEFT_MARGIN = 90.0f, RIGHT_MARGIN = 124.0f, TOTAL_WIDTH = 1080 - LEFT_MARGIN - RIGHT_MARGIN;

    size_t count = cards.size();
    float spacing = MAX_SPACE;

    if (count > 1)
    {
        float requiredWidth = CARD_WIDTH * count + (count - 2) * MAX_SPACE + TOP_GAP;
        if (requiredWidth > TOTAL_WIDTH) spacing = std::max(MIN_SPACE, (TOTAL_WIDTH - CARD_WIDTH * count - TOP_GAP) / (count - 2));
    }

    float usedWidth = CARD_WIDTH * count + spacing * (count - 2) + (count > 1 ? TOP_GAP : 0);
    float scaleFactor = (usedWidth > TOTAL_WIDTH ? TOTAL_WIDTH / usedWidth : 1.0f);
    usedWidth = std::min(usedWidth, TOTAL_WIDTH);

    float startX = LEFT_MARGIN + (TOTAL_WIDTH - usedWidth) / 2 + CARD_WIDTH * scaleFactor / 2;

    for (size_t idx = 0; idx < count; ++idx)
    {
        CardModel* cardModel = cards[idx];
        CardView* cardView = nullptr;
        if (_cardViews.count(cardModel) == 0)
        {
            cardView = CardView::create(cardModel);
            _cardViews[cardModel] = cardView;
            addChild(cardView);
        }
        else cardView = _cardViews[cardModel];

        float posX = startX + idx * (CARD_WIDTH * scaleFactor + spacing * scaleFactor);
        if (idx == count - 1 && count > 1) posX += TOP_GAP * scaleFactor;

        cardView->setPosition(Vec2(posX, BASE_Y));
        cardView->setScale(scaleFactor);
        cardView->setLocalZOrder(100 + idx);
        cardView->refresh();
    }
}

void GameView::showUndoButton(const std::function<void()>& callback)
{
    auto undoBtn = Button::create();
    undoBtn->setTitleText("Back");
    undoBtn->setTitleFontSize(70);
    undoBtn->setTitleColor(Color3B::WHITE);
    undoBtn->setScale9Enabled(true);
    undoBtn->setContentSize(Size(180, 100));
    undoBtn->setOpacity(200);
    undoBtn->setPosition(Vec2(1080 - 70, 520));
    undoBtn->addClickEventListener([callback](Ref*) { if (callback) callback(); });
    addChild(undoBtn, 999);
}

void GameView::moveCardWithAnimation(CardModel* cardModel, const Vec2& targetPos, std::function<void()> onFinish)
{
    CardView* cardView = getCardView(cardModel);
    if (!cardView) return;

    auto moveAction = MoveTo::create(0.3f, targetPos);
    if (onFinish)
    {
        auto seq = Sequence::create(moveAction, CallFunc::create(onFinish), nullptr);
        cardView->runAction(seq);
    }
    else cardView->runAction(moveAction);
}

CardView* GameView::getCardView(CardModel* cardModel) const
{
    auto it = _cardViews.find(cardModel);
    return (it != _cardViews.end()) ? it->second : nullptr;
}

void GameView::clearCardViews()
{
    for (auto& kv : _cardViews)
        if (kv.second) kv.second->removeFromParent();
    _cardViews.clear();
}


