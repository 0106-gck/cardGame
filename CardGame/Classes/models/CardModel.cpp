#include "CardModel.h"

// 静态工厂方法：创建并初始化一张牌
CardModel* CardModel::create(int face, int suit, const cocos2d::Vec2& pos, bool isFaceUp)
{
    CardModel* card = new CardModel();
    if (card && card->init(face, suit, pos, isFaceUp))
    {
        card->autorelease();
        return card;
    }
    CC_SAFE_DELETE(card); // 安全释放内存
    return nullptr;
}

// 实际初始化函数
bool CardModel::init(int face, int suit, const cocos2d::Vec2& pos, bool isFaceUp)
{
    // 检查点数和花色是否合法
    if (face < 0 || face > 13) return false;
    if (suit < 0 || suit > 3) return false;

    _face = face;
    _suit = suit;
    _isFaceUp = isFaceUp;
    _cardPos = pos;
    return true;
}

