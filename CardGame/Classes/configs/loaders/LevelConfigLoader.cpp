#include "LevelConfigLoader.h"
#include "json/document.h"
#include "cocos2d.h"

LevelConfig* LevelConfigLoader::loadFromFile(const std::string& jsonPath) {
    using namespace cocos2d;

    // ��ȡ�ļ�
    std::string jsonData = FileUtils::getInstance()->getStringFromFile(jsonPath);
    if (jsonData.empty()) return nullptr;

    rapidjson::Document doc;
    doc.Parse(jsonData.c_str());
    if (doc.HasParseError()) return nullptr;

    LevelConfig* config = new LevelConfig();

    // ���� Playfield
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        for (auto& cardItem : doc["Playfield"].GetArray()) {
            CardConfig cardCfg;
            cardCfg.face = cardItem["CardFace"].GetInt();
            cardCfg.suit = cardItem["CardSuit"].GetInt();
            cardCfg.pos.x = cardItem["Position"]["x"].GetFloat();
            cardCfg.pos.y = cardItem["Position"]["y"].GetFloat();
            config->playfieldCards.push_back(cardCfg);
        }
    }

    // ���� Stack
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        for (auto& cardItem : doc["Stack"].GetArray()) {
            CardConfig cardCfg;
            cardCfg.face = cardItem["CardFace"].GetInt();
            cardCfg.suit = cardItem["CardSuit"].GetInt();
            cardCfg.pos.x = cardItem["Position"]["x"].GetFloat();
            cardCfg.pos.y = cardItem["Position"]["y"].GetFloat();
            config->stackCards.push_back(cardCfg);
        }
    }

    return config;
}
