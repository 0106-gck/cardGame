#include "GameController.h"
#include "ui/CocosGUI.h"

bool GameController::init()
{
    if (!Scene::init()) return false;

    // 获取主数据模型并加载关卡
    _model = GameModel::getInstance();
    _model->loadLevel("level/level1.json");

    // 初始化 Undo 数据与管理器
    _undoModel = new UndoModel();
    _undoModel->retain();
    _undoManager = new UndoManager(_undoModel);

    // 创建视图
    _view = GameView::create();
    addChild(_view);

    // 初始渲染
    _view->renderPlayfield(_model->getPlayfield());
    _view->renderStack(_model->getStack());

    // 注册牌点击监听器
    registerCardListeners();

    // 显示回退按钮并绑定回调
    _view->showUndoButton(CC_CALLBACK_0(GameController::onUndoClicked, this));
    return true;
}

// 注册所有可交互牌的监听器
void GameController::registerCardListeners()
{
    auto dispatcher = _view->getEventDispatcher();

    // 移除已有监听器，避免重复
    for (auto* card : _model->getPlayfield()) {
        CardView* view = _view->getCardView(card);
        if (view)
            dispatcher->removeEventListenersForTarget(view, true);
    }
    for (auto* card : _model->getStack()) {
        CardView* view = _view->getCardView(card);
        if (view)
            dispatcher->removeEventListenersForTarget(view, true);
    }

    // 手牌区——非顶牌可点
    auto& stack = _model->getStack();
    if (stack.size() > 1) {
        for (size_t idx = 0; idx < stack.size() - 1; ++idx) {
            CardModel* card = stack[idx];
            CardView* view = _view->getCardView(card);
            if (view) {
                auto listener = cocos2d::EventListenerTouchOneByOne::create();
                listener->onTouchBegan = [this, view, card](cocos2d::Touch* touch, cocos2d::Event*) {
                    if (view->getBoundingBox().containsPoint(touch->getLocation())) {
                        this->onStackCardClicked(card);
                        return true;
                    }
                    return false;
                    };
                dispatcher->addEventListenerWithSceneGraphPriority(listener, view);
            }
        }
    }

    // 桌面区——所有牌可点
    for (auto* card : _model->getPlayfield()) {
        CardView* view = _view->getCardView(card);
        if (view) {
            auto listener = cocos2d::EventListenerTouchOneByOne::create();
            listener->onTouchBegan = [this, view, card](cocos2d::Touch* touch, cocos2d::Event*) {
                if (view->getBoundingBox().containsPoint(touch->getLocation())) {
                    this->onPlayfieldCardClicked(card);
                    return true;
                }
                return false;
                };
            dispatcher->addEventListenerWithSceneGraphPriority(listener, view);
        }
    }
}

// 点击手牌区牌：将牌移到顶部并记录操作
void GameController::onStackCardClicked(CardModel* card)
{
    auto& stack = _model->getStack();
    if (stack.size() < 2) return;
    CardModel* topCard = stack.back();
    if (card == topCard) return;

    size_t fromIdx = std::find(stack.begin(), stack.end(), card) - stack.begin();
    cocos2d::Vec2 fromPos = _view->getCardView(card)->getPosition();
    cocos2d::Vec2 toPos = _view->getCardView(topCard)->getPosition();

    // 记录被替换的顶部牌
    CardModel* replaced = topCard;
    cocos2d::Vec2 replacedPos = _view->getCardView(replaced)->getPosition();

    CardMoveOp op;
    op.card = card;
    op.from = fromPos;
    op.to = toPos;
    op.fromArea = 0;
    op.toArea = 0;
    op.fromIndex = fromIdx;
    op.replacedCard = replaced;
    op.replacedIndex = fromIdx;
    op.replacedFrom = replacedPos;
    _undoManager->recordMove(op);

    // 移动到顶部
    stack.erase(stack.begin() + fromIdx);
    stack.push_back(card);

    _view->moveCardWithAnimation(card, toPos, [this]() {
        _view->renderStack(_model->getStack());
        registerCardListeners();
        });
}

// 点击桌面区牌：移动到手牌顶部并记录操作
void GameController::onPlayfieldCardClicked(CardModel* card)
{
    auto& stack = _model->getStack();
    if (stack.empty()) return;
    CardModel* topCard = stack.back();

    if (abs(topCard->getFace() - card->getFace()) == 1) {
        cocos2d::Vec2 fromPos = _view->getCardView(card)->getPosition();
        cocos2d::Vec2 toPos = _view->getCardView(topCard)->getPosition();

        CardMoveOp op;
        op.card = card;
        op.from = fromPos;
        op.to = toPos;
        op.fromArea = 1;
        op.toArea = 0;
        op.fromIndex = -1;
        op.replacedCard = nullptr;
        op.replacedIndex = -1;
        op.replacedFrom = cocos2d::Vec2::ZERO;
        _undoManager->recordMove(op);

        auto& playfield = _model->getPlayfield();
        playfield.erase(std::find(playfield.begin(), playfield.end(), card));
        stack.push_back(card);

        _view->moveCardWithAnimation(card, toPos, [this]() {
            _view->renderPlayfield(_model->getPlayfield());
            _view->renderStack(_model->getStack());
            registerCardListeners();
            });
    }
}

// 回退按钮点击处理
void GameController::onUndoClicked()
{
    if (!_undoManager->canUndo()) return;
    CardMoveOp op = _undoManager->undo();
    if (!op.card) return;

    auto& stack = _model->getStack();
    auto& playfield = _model->getPlayfield();

    // 从当前位置移除被移动牌
    if (op.toArea == 0) {
        auto it = std::find(stack.begin(), stack.end(), op.card);
        if (it != stack.end()) stack.erase(it);

        // 手牌区交换操作
        if (op.fromArea == 0 && op.replacedCard) {
            auto it2 = std::find(stack.begin(), stack.end(), op.replacedCard);
            if (it2 != stack.end()) stack.erase(it2);

            // 恢复位置
            op.card->setCardPos(op.from);
            op.replacedCard->setCardPos(op.replacedFrom);

            // 插回原位置
            stack.insert(stack.begin() + op.fromIndex, op.card);
            stack.push_back(op.replacedCard);

            _view->moveCardWithAnimation(op.card, op.from, [this]() {
                _view->renderStack(_model->getStack());
                registerCardListeners();
                });
            return;
        }
    }
    else if (op.toArea == 1) {
        auto it = std::find(playfield.begin(), playfield.end(), op.card);
        if (it != playfield.end()) playfield.erase(it);
    }

    // 其它情况
    op.card->setCardPos(op.from);
    if (op.fromArea == 0) stack.push_back(op.card);
    else if (op.fromArea == 1) playfield.push_back(op.card);

    _view->moveCardWithAnimation(op.card, op.from, [this]() {
        _view->renderPlayfield(_model->getPlayfield());
        _view->renderStack(_model->getStack());
        registerCardListeners();
        });
}
