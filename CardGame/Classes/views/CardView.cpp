#include "CardView.h"
USING_NS_CC;

CardView* CardView::create(CardModel* model)
{
    CardView* newCard = new (std::nothrow) CardView();
    if (newCard && newCard->init(model))
    {
        newCard->autorelease();
        return newCard;
    }
    CC_SAFE_DELETE(newCard);
    return nullptr;
}

bool CardView::init(CardModel* model)
{
    if (!Sprite::initWithFile("card_general.png"))
        return false;

    _model = model;
    if (_model)
        setPosition(_model->getCardPos());

    refresh();
    return true;
}

void CardView::setModel(CardModel* model)
{
    _model = model;
    refresh();
}

void CardView::refresh()
{
    removeAllChildren();
    if (!_model || !_model->isFaceUp()) return;

    int suitId = _model->getSuit();
    int faceId = _model->getFace();

    if (suitId < 0 || suitId > 3 || faceId < 0 || faceId > 12) return;

    std::string colorStr = (suitId == CST_DIAMONDS || suitId == CST_HEARTS) ? "red" : "black";

    std::string faceStr;
    if (faceId == 0) faceStr = "A";
    else if (faceId == 10) faceStr = "J";
    else if (faceId == 11) faceStr = "Q";
    else if (faceId == 12) faceStr = "K";
    else faceStr = std::to_string(faceId + 1);

    const char* suitNamesArr[] = { "club", "diamond", "heart", "spade" };
    std::string suitPath = "suits/" + std::string(suitNamesArr[suitId]) + ".png";

    auto smallNumSprite = Sprite::create("number/small_" + colorStr + "_" + faceStr + ".png");
    if (smallNumSprite)
    {
        smallNumSprite->setAnchorPoint(Vec2(0, 1));
        smallNumSprite->setPosition(Vec2(10, getContentSize().height - 10));
        addChild(smallNumSprite);
    }

    auto suitSprite = Sprite::create(suitPath);
    if (suitSprite)
    {
        suitSprite->setAnchorPoint(Vec2(1, 1));
        suitSprite->setPosition(Vec2(getContentSize().width - 10, getContentSize().height - 10));
        addChild(suitSprite);
    }

    auto bigNumSprite = Sprite::create("number/big_" + colorStr + "_" + faceStr + ".png");
    if (bigNumSprite)
    {
        bigNumSprite->setAnchorPoint(Vec2(0.5, 0));
        bigNumSprite->setPosition(Vec2(getContentSize().width / 2, 10));
        addChild(bigNumSprite);
    }
}
