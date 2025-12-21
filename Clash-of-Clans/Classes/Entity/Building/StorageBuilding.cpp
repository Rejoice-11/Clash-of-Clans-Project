#include "StorageBuilding.h"
#include "Classes/Data/BuildingData.h"
#include "cocos2d.h"

// 全局计数（初始化为0）
int countofGoldStoragesInVillage = 0;
int countofElixirStoragesInVillage = 0;

// 建筑数据定义（与 ResourceBuilding 风格一致）
BuildingData GoldStorageBuildingData = {
    4, // id
    BuildingType::GOLD_STORAGE,
    1, // 初始等级
    {150, 400, 800, 1500, 2500}, // maxBuildCount (最多5个)
    {2, 3, 4, 5, 5}, // goldCost
    {0, 0, 0, 0, 0},             // elixirCost
    {30, 60, 120, 180, 240},     // buildTime (seconds)
    {800, 1200, 1800, 2500, 3500}, // hitPoints
    {}, // productionPerHour (留空，存储建筑不产出)
    {5000, 10000, 20000, 30000, 50000}, // capacity (新增字段！)
    {},
    {}
};

BuildingData ElixirStorageBuildingData = {
    5, // id
    BuildingType::ELIXIR_STORAGE,
    1,
    {2, 3, 4, 5, 5},
    { 150, 400, 800, 1500, 2500 },             // goldCost
    {0, 0, 0, 0, 0} , // elixirCost
    {30, 60, 120, 180, 240},
    {800, 1200, 1800, 2500, 3500},
    {},
    {5000, 10000, 20000, 30000, 50000}, // capacity
    {},
    {}
};

// 构造函数
StorageBuilding::StorageBuilding(const BuildingData& data, int instanceId, StorageType type)
    : Building(data, instanceId)
    , _storageType(type) {
    // 初始化时增加全局计数
    if (instanceId == -1) {
        static int nextId = 2001; // 从2001开始（避免和TownHall冲突）
        setId(nextId++);
    }

    if (_storageType == StorageType::GOLD) {
        countofGoldStoragesInVillage++;
    }
    else {
        countofElixirStoragesInVillage++;
    }
}

// ———————— 重写基类虚函数 ————————

cocos2d::Sprite* StorageBuilding::createSprite() {
    std::string frameName = getSpriteFrameName();
    auto sprite = cocos2d::Sprite::create(frameName + ".png");
    if (!sprite) {
        // 回退到默认图
        std::string fallback = (_storageType == StorageType::GOLD) ? "gold_storage_lv1.png" : "elixir_storage_lv1.png";
        sprite = cocos2d::Sprite::create(fallback);
    }
    return sprite;
}

void StorageBuilding::upgrade() {
    if (_currentLevel < MAX_LEVELS) {
        _currentLevel++;
        // 可在此触发UI更新（比如刷新ResourceManager的maxStorage）
    }
}

std::string StorageBuilding::getSpriteFrameName() const {
    std::string baseName = (_storageType == StorageType::GOLD) ? "gold_storage_lv" : "elixir_storage_lv";
    return baseName + std::to_string(_currentLevel);
}

// ———————— 存储特有逻辑 ————————

int StorageBuilding::getCapacity() const {
    if (_currentLevel < 0 || _currentLevel >= MAX_LEVELS) {
        return 0;
    }
    return _data.capacity[_currentLevel - 1];
}