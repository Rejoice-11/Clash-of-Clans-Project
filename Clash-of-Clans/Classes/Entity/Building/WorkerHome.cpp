// WorkerHome.cpp
#include "Classes/Entity/Building/WorkerHome.h"
#include "cocos2d.h"

// 建筑数据定义（仿 MilitaryBuildingBuildingData）
BuildingData WorkerHomeBuildingData =
{
    5, // id
    BuildingType::WORKER_HOME, // 建筑类型（需在 BuildingType 中定义）
    1, // 初始等级
    {1, 1, 2, 2, 3}, // maxBuildCount (Lv1~5)
    {150, 400, 800, 1600, 2500}, // goldCost
    {0, 0, 0, 0, 0}, // elixirCost（假设只用金币）
    {45, 90, 180, 240, 300}, // buildTime (seconds)
    {300, 600, 1200, 2000, 3000}, // hitPoints
    {}, // productionPerHour（不生产资源）
    {1, 1, 1, 1, 1}, // workerCapacity（关键字段：每级工人数量）
    {},
    {}
};

WorkerHome::WorkerHome(const BuildingData& data, int instanceId)
    : Building(data, instanceId) 
{
    if (instanceId == -1) 
    {
        static int nextId = 8001; // 工人小屋ID起始
        setId(nextId++);
    }
}

void WorkerHome::upgrade() {
    if (_currentLevel < MAX_LEVELS) {
        _currentLevel++;
        // TODO: 触发 UI 更新（如 VillageScene 刷新建造队列）
    }
}

int WorkerHome::getWorkerCount() const {
    if (_currentLevel <= 0 || _currentLevel > MAX_LEVELS) {
        return 0;
    }
    // 注意：workerCapacity 存储在 BuildingData 的 capacity 字段
    return WorkerHomeBuildingData.capacity[_currentLevel - 1];
}

cocos2d::Sprite* WorkerHome::createSprite() {
    std::string frameName = getSpriteFrameName();
    auto sprite = cocos2d::Sprite::create(frameName + ".png");
    if (!sprite) {
        sprite = cocos2d::Sprite::create("worker_home_lv1.png"); // 默认 fallback
    }
    return sprite;
}

std::string WorkerHome::getSpriteFrameName() const {
    return "worker_home_lv" + std::to_string(_currentLevel);
}