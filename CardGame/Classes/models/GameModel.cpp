#include "GameModel.h"
#include "configs/models/LevelConfig.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "cocos2d.h"
#include <algorithm>

GameModel* GameModel::getInstance()
{
    static GameModel instance;
    return &instance;
}

void GameModel::clear()
{
    for (auto* card : _playfield) card->release();
    for (auto* card : _stack) card->release();
    _playfield.clear();
    _stack.clear();
}

// �½ӿڣ�ͨ�� LevelConfig ֱ�Ӽ���
void GameModel::loadLevelConfig(LevelConfig* config)
{
    clear();
    if (!config) return;

    // ������
    for (const auto& cfg : config->playfieldCards)
    {
        CardModel* card = CardModel::create(cfg.face, cfg.suit, cfg.pos, true);
        cocos2d::Vec2 pos = card->getCardPos();
        pos.y += 200.0f;
        card->setCardPos(pos);

        card->retain();
        _playfield.push_back(card);
    }

    // ������
    for (const auto& cfg : config->stackCards)
    {
        CardModel* card = CardModel::create(cfg.face, cfg.suit, cfg.pos, true);
        card->retain();
        _stack.push_back(card);
    }
}

// �ɽӿڣ�ͨ�� LevelConfigLoader ����
void GameModel::loadLevel(const std::string& jsonPath)
{
    LevelConfig* cfg = LevelConfigLoader::loadFromFile(jsonPath);
    loadLevelConfig(cfg);
    if (cfg) delete cfg;
}

std::vector<CardModel*>& GameModel::getPlayfield() { return _playfield; }
std::vector<CardModel*>& GameModel::getStack() { return _stack; }

void GameModel::moveCardToPlayfield(CardModel* card, const cocos2d::Vec2& toPos)
{
    auto it = std::find(_stack.begin(), _stack.end(), card);
    if (it != _stack.end())
        _stack.erase(it);

    card->setCardPos(toPos);
    _playfield.push_back(card);
}

void GameModel::moveCardToStack(CardModel* card, const cocos2d::Vec2& toPos)
{
    auto it = std::find(_playfield.begin(), _playfield.end(), card);
    if (it != _playfield.end())
        _playfield.erase(it);

    card->setCardPos(toPos);
    _stack.push_back(card);
}
