#include "GameController.h"
#include "ui/CocosGUI.h"

bool GameController::init()
{
    if (!Scene::init()) return false;

    // ��ȡ������ģ�Ͳ����عؿ�
    _model = GameModel::getInstance();
    _model->loadLevel("level/level1.json");

    // ��ʼ�� Undo �����������
    _undoModel = new UndoModel();
    _undoModel->retain();
    _undoManager = new UndoManager(_undoModel);

    // ������ͼ
    _view = GameView::create();
    addChild(_view);

    // ��ʼ��Ⱦ
    _view->renderPlayfield(_model->getPlayfield());
    _view->renderStack(_model->getStack());

    // ע���Ƶ��������
    registerCardListeners();

    // ��ʾ���˰�ť���󶨻ص�
    _view->showUndoButton(CC_CALLBACK_0(GameController::onUndoClicked, this));
    return true;
}

// ע�����пɽ����Ƶļ�����
void GameController::registerCardListeners()
{
    auto dispatcher = _view->getEventDispatcher();

    // �Ƴ����м������������ظ�
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

    // �����������Ƕ��ƿɵ�
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

    // ���������������ƿɵ�
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

// ����������ƣ������Ƶ���������¼����
void GameController::onStackCardClicked(CardModel* card)
{
    auto& stack = _model->getStack();
    if (stack.size() < 2) return;
    CardModel* topCard = stack.back();
    if (card == topCard) return;

    size_t fromIdx = std::find(stack.begin(), stack.end(), card) - stack.begin();
    cocos2d::Vec2 fromPos = _view->getCardView(card)->getPosition();
    cocos2d::Vec2 toPos = _view->getCardView(topCard)->getPosition();

    // ��¼���滻�Ķ�����
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

    // �ƶ�������
    stack.erase(stack.begin() + fromIdx);
    stack.push_back(card);

    _view->moveCardWithAnimation(card, toPos, [this]() {
        _view->renderStack(_model->getStack());
        registerCardListeners();
        });
}

// ����������ƣ��ƶ������ƶ�������¼����
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

// ���˰�ť�������
void GameController::onUndoClicked()
{
    if (!_undoManager->canUndo()) return;
    CardMoveOp op = _undoManager->undo();
    if (!op.card) return;

    auto& stack = _model->getStack();
    auto& playfield = _model->getPlayfield();

    // �ӵ�ǰλ���Ƴ����ƶ���
    if (op.toArea == 0) {
        auto it = std::find(stack.begin(), stack.end(), op.card);
        if (it != stack.end()) stack.erase(it);

        // ��������������
        if (op.fromArea == 0 && op.replacedCard) {
            auto it2 = std::find(stack.begin(), stack.end(), op.replacedCard);
            if (it2 != stack.end()) stack.erase(it2);

            // �ָ�λ��
            op.card->setCardPos(op.from);
            op.replacedCard->setCardPos(op.replacedFrom);

            // ���ԭλ��
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

    // �������
    op.card->setCardPos(op.from);
    if (op.fromArea == 0) stack.push_back(op.card);
    else if (op.fromArea == 1) playfield.push_back(op.card);

    _view->moveCardWithAnimation(op.card, op.from, [this]() {
        _view->renderPlayfield(_model->getPlayfield());
        _view->renderStack(_model->getStack());
        registerCardListeners();
        });
}
