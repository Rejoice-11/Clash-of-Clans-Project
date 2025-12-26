#include "ConfigManagerUnit.h"
#include "json/document.h"
#include "cocos2d.h"

USING_NS_CC;

ConfigManagerUnit* ConfigManagerUnit::_instance = nullptr;

ConfigManagerUnit* ConfigManagerUnit::getInstance() {
    if (!_instance) _instance = new ConfigManagerUnit();
    return _instance;
}

bool ConfigManagerUnit::loadConfigs(const std::string& filename) {
    // 1. 获取文件内容
    std::string jsonPath = FileUtils::getInstance()->fullPathForFilename(filename);
    std::string content = FileUtils::getInstance()->getStringFromFile(jsonPath);

    if (content.empty()) {
        CCLOG("ConfigManagerUnit: Error reading file %s", filename.c_str());
        return false;
    }

    // 2. 解析 JSON
    rapidjson::Document doc;
    doc.Parse(content.c_str());

    if (doc.HasParseError()) {
        CCLOG("ConfigManagerUnit: JSON parse error!");
        return false;
    }

    // 3. 遍历并将数据存入 Map
    if (doc.HasMember("units") && doc["units"].IsArray()) {
        const auto& units = doc["units"];
        for (rapidjson::SizeType i = 0; i < units.Size(); i++) {
            const auto& item = units[i];

            UnitData data;
            data.id = item["id"].GetInt();
			data.type = static_cast<UnitType>(item["type"].GetInt());
			data.costForHavingOne = item["costForHavingOne"].GetInt();
            data.hp = item["hp"].GetInt();
            data.damage = item["damage"].GetInt();
            data.attackRange = item["attackRange"].GetFloat();
            data.attackInterval = item["attackInterval"].GetFloat();
            data.speed = item["speed"].GetFloat();
            
            

            _unitConfigs[data.id] = data;
        }
    }

    CCLOG("ConfigManager: Successfully loaded %lu units.", _unitConfigs.size());
    return true;
}

const UnitData& ConfigManagerUnit::getUnitData(int id) {
    if (_unitConfigs.find(id) != _unitConfigs.end()) {
        return _unitConfigs[id];
    }
    CCLOG("ConfigManagerUnit: Warning! Unit ID %d not found.", id);
    return _defaultData;
}