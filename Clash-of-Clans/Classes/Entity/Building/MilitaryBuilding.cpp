// MilitaryBuilding.cpp
#include "Classes/Entity/Building/MilitaryBuilding.h"
#include "cocos2d.h"

BuildingData MilitaryBuildingBuildingData = 
{
    4, // id
    BuildingType::MILITARY_CAMP, // 假设 BuildingType 新增 BARRACKS=4
    1, // 初始等级
    {1, 1, 1, 1, 1}, // maxBuildCount (Lv1~5)
    {200, 500, 1000, 2000, 3000}, // goldCost
    {0, 0, 0, 0, 0}, // elixirCost (假设只用金币)
    {60, 120, 240, 300, 400}, // buildTime (seconds)
    {500, 1000, 2000, 3500, 5000}, // hitPoints
    {}, // productionPerHour (可选，用于训练速度)
    { 50, 70, 90, 120, 180 }, // troopCapacity ← 关键字段！
    {},
    {}
 };

MilitaryBuilding::MilitaryBuilding(const BuildingData& data, int instanceId)
    : Building(data, instanceId) 
{
    // 可以在这里做额外初始化
    if (instanceId == -1)
    {
        // 自动生成ID
        static int nextId = 7001;
        setId(nextId++);
    }
}

void MilitaryBuilding::upgrade()
{
    if (_currentLevel < MAX_LEVELS)
    {
        _currentLevel++;
        // TODO: 触发 UI 更新（后续由 VillageScene 处理）
    }
}

int MilitaryBuilding::getTotalTroopCapacity() const 
{
    if (_currentLevel <= 0 || _currentLevel > MAX_LEVELS) 
    {
        return 0;
    }
    return _data.capacity[_currentLevel - 1]; // troopCapacity[0] = Lv1 容量
}

cocos2d::Sprite* MilitaryBuilding::createSprite()
{
    std::string frameName = getSpriteFrameName();
    auto sprite = cocos2d::Sprite::create(frameName + ".png");
    if (!sprite) 
    {
        sprite = cocos2d::Sprite::create("millitary_camp_lv1.png"); // 默认 fallback
    }
    return sprite;
}

std::string MilitaryBuilding::getSpriteFrameName() const 
{
    return "millitary_camp_lv" + std::to_string(_currentLevel);
}