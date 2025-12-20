// Classes/Core/ConfigManager.cpp
#include "ConfigManager.h"
#include "platform/CCFileUtils.h"
#include "json/document-wrapper.h"

ConfigManager* ConfigManager::s_instance = nullptr;

ConfigManager* ConfigManager::getInstance()
{
    if (!s_instance)
    {
        s_instance = new (std::nothrow) ConfigManager();
    }
    return s_instance;
}

void ConfigManager::destroyInstance()
{
    CC_SAFE_DELETE(s_instance);
}

ConfigManager::ConfigManager()
{
    loadDefaultData();
}

ConfigManager::~ConfigManager()
{
}

void ConfigManager::loadDefaultData()
{
    // 简单示例数据。后续可由 JSON 配置替换。
    BuildingData townhall;
    townhall.id = 0;
    townhall.cost_gold = 0;
    townhall.cost_elixir = 0;
    townhall.level = 1;
    _buildingMap[townhall.id] = townhall;

    BuildingData goldMine;
    goldMine.id = 1;
    goldMine.cost_gold = 200;
    goldMine.cost_elixir = 0;
    goldMine.level = 1;
    _buildingMap[goldMine.id] = goldMine;

    BuildingData archerTower;
    archerTower.id = 2;
    archerTower.cost_gold = 500;
    archerTower.cost_elixir = 100;
    archerTower.level = 1;
    _buildingMap[archerTower.id] = archerTower;
}

const BuildingData& ConfigManager::getBuildingData(int buildingId) const
{
    auto it = _buildingMap.find(buildingId);
    if (it != _buildingMap.end())
        return it->second;

    static BuildingData s_empty;
    return s_empty;
}

bool ConfigManager::loadFromJson(const std::string& filepath)
{
    // 简单实现：尝试用 FileUtils 读取 ValueMap（支持特定 json 格式或 plist）
    // 这里保留接口以便后续扩展，当前不覆盖默认数据。
    if (filepath.empty()) return false;

    ValueMap vm = FileUtils::getInstance()->getValueMapFromFile(filepath);
    if (vm.empty()) return false;

    // 期望格式:
    // { "buildings": { "0": { "cost_gold":0, "cost_elixir":0, "level":1 }, "1": { ... } } }
    auto it = vm.find("buildings");
    if (it == vm.end() || it->second.getType() != Value::Type::MAP) return false;

    ValueMap buildings = it->second.asValueMap();
    for (const auto& kv : buildings)
    {
        int id = std::stoi(kv.first);
        if (kv.second.getType() != Value::Type::MAP) continue;
        ValueMap bd = kv.second.asValueMap();
        BuildingData b;
        b.id = id;
        if (bd.find("cost_gold") != bd.end()) b.cost_gold = bd["cost_gold"].asInt();
        if (bd.find("cost_elixir") != bd.end()) b.cost_elixir = bd["cost_elixir"].asInt();
        if (bd.find("level") != bd.end()) b.level = bd["level"].asInt();
        _buildingMap[id] = b;
    }

    return true;
}