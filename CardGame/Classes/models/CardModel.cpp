#include "CardModel.h"

// ��̬������������������ʼ��һ����
CardModel* CardModel::create(int face, int suit, const cocos2d::Vec2& pos, bool isFaceUp)
{
    CardModel* card = new CardModel();
    if (card && card->init(face, suit, pos, isFaceUp))
    {
        card->autorelease();
        return card;
    }
    CC_SAFE_DELETE(card); // ��ȫ�ͷ��ڴ�
    return nullptr;
}

// ʵ�ʳ�ʼ������
bool CardModel::init(int face, int suit, const cocos2d::Vec2& pos, bool isFaceUp)
{
    // �������ͻ�ɫ�Ƿ�Ϸ�
    if (face < 0 || face > 13) return false;
    if (suit < 0 || suit > 3) return false;

    _face = face;
    _suit = suit;
    _isFaceUp = isFaceUp;
    _cardPos = pos;
    return true;
}

