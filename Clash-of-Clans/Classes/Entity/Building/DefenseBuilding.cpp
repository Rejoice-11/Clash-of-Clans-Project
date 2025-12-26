#include "DefenseBuilding.h"
#include "Classes/Data/BuildingData.h"
#include "cocos2d.h"

// 全局计数（初始化为0）
int countofArcherTowersInVillage = 0;
int countofCanonsInVillage = 0;

// 建筑数据定义（与 ResourceBuilding 风格一致）

BuildingData ArcherTowerBuildingData = {
    7, // id
    BuildingType::ARCHER_TOWER,
    1,
    {2, 3, 4, 5, 6}, // maxBuildCount
    {800, 1600, 3200, 6400, 12000}, // goldCost
    {0, 0, 0, 0, 0},                 // elixirCost
    {120, 240, 480, 960, 1920},      // buildTime (seconds)
    {1200, 2000, 3000, 4500, 6500},  // hitPoints
    {}, // productionPerHour
    {}, // capacity
    {8, 10, 12, 14, 16},             // attackRange (新增字段！)
    {20.0f, 30.0f, 45.0f, 65.0f, 90.0f} // damagePerSecond (新增字段！)
};

BuildingData CanonBuildingData = {
    8, // id
    BuildingType::CANNON,
    1,
    {2, 3, 4, 5, 6},
    {0, 0, 0, 0, 0},                 // goldCost
    {800, 1600, 3200, 6400, 12000}, // elixirCost
    {120, 240, 480, 960, 1920},
    {1500, 2500, 4000, 6000, 9000},
    {},
    {},
    {7, 9, 11, 13, 15},              // attackRange
    {35.0f, 50.0f, 75.0f, 110.0f, 160.0f} // damagePerSecond
};

// 构造函数
DefenseBuilding::DefenseBuilding(const BuildingData& data, int instanceId, DefenseType type)
    : Building(data, instanceId)
    , _defenseType(type) {
    if (instanceId == -1) {
        static int nextId = 4001; // 从4001开始（避免ID冲突）
        setId(nextId++);
    }

    // 分别计数
    if (_defenseType == DefenseType::ARCHER_TOWER) {
        countofArcherTowersInVillage++;
    }
    else {
        countofCanonsInVillage++;
    }
}

// ———————— 重写基类虚函数 ————————

cocos2d::Sprite* DefenseBuilding::createSprite() 
{
    std::string frameName = getSpriteFrameName();
    auto sprite = cocos2d::Sprite::create(frameName + ".png");
    if (!sprite) 
    {
        // 回退到默认图
        std::string fallback = (_defenseType == DefenseType::ARCHER_TOWER)
            ? "archer_tower_lv1.png" : "canon_lv1.png";
        sprite = cocos2d::Sprite::create(fallback);
    }
    return sprite;
}

void DefenseBuilding::upgrade() 
{
    if (_currentLevel < MAX_LEVELS) 
    {
        _currentLevel++;
        // 可在此触发战斗系统更新射程/DPS
    }
}

std::string DefenseBuilding::getSpriteFrameName() const 
{
    if (_defenseType == DefenseType::ARCHER_TOWER)
    {
        return "archer_tower_lv" + std::to_string(_currentLevel);
    }
    else 
    {
        return "canon_lv" + std::to_string(_currentLevel);
    }
}

// ———————— 防御特有逻辑 ————————

float DefenseBuilding::getAttackRange() const 
{
    if (_currentLevel < 0 || _currentLevel >= MAX_LEVELS ) 
    {
        return 0.0f;
    }
    return static_cast<float>(_data.attackRange[_currentLevel + 1]);
}

float DefenseBuilding::getDamagePerSecond() const 
{
    if (_currentLevel < 0 || _currentLevel >= MAX_LEVELS)
    {
        return 0.0f;
    }
    return _data.damagePerSecond[_currentLevel + 1];
}