#include "Classes/Entity/Building/TownHall.h"
#include "Classes/Data/BuildingData.h"
#include "cocos2d.h"

// 定义全局变量（只能在这里定义一次！）
int countofTownHallsInVillage = 0;

// 初始化配置数据（建议从 JSON 加载，但先用硬编码）
BuildingData TownHallBuildingData = 
{
    1, // id
    BuildingType::TOWN_HALL,
    1, // level
    {1, 1, 1, 1, 1}, // maxBuildCount: 只允许一个大本营
    {1000, 2000, 4000, 6000, 9000}, // goldCost
    {0, 0, 0, 0, 0}, // elixirCost
    {60, 120, 240, 300, 400}, // buildTime (seconds)
    {2000, 3000, 5000, 7000, 10000}, // hitPoints
    {},
    {},
    {},
    {}
};

// 构造函数实现
TownHall::TownHall(const BuildingData& data, int instanceId)
    : Building(data, instanceId)
{
    // 可以在这里做额外初始化
    if (instanceId == -1) 
    {
        // 自动生成ID
        static int nextId = 1001;
        setId(nextId++);
    }
}

// 创建精灵
cocos2d::Sprite* TownHall::createSprite() {
    auto sprite = cocos2d::Sprite::create("town_hall.png"); // 先用统一图
    sprite->setScale(1.2f);
    return sprite;
}

// 升级逻辑
void TownHall::upgrade() {
    if (_currentLevel < MAX_LEVELS) 
    {
        _currentLevel++;
        // 触发UI刷新、播放动画等
    }
}

// 获取精灵名（未来支持多级）
std::string TownHall::getSpriteFrameName() const {
    return "town_hall_lv" + std::to_string(_currentLevel);
}