#include "ArmyManager.h"

ArmyManager* ArmyManager::_instance = nullptr;

ArmyManager* ArmyManager::getInstance() {
    if (!_instance) _instance = new ArmyManager();
    return _instance;
}

void ArmyManager::setUnitCount(UnitType id, int count) {
    _armyPool[id] = count;
}

int ArmyManager::getRemainingCount(UnitType id) {
    if (_armyPool.find(id) != _armyPool.end()) {
        return _armyPool[id];
    }
    return 0;
}

bool ArmyManager::tryDeploy(UnitType id) {
    if (getRemainingCount(id) > 0) {
        _armyPool[id]--;
        return true;
    }
    return false;
}

void ArmyManager::getRemainingCapacity()
{
    for (int i = 0; i < 4; i++)
    {
        _armyPool[(UnitType)i] = _armyPool[(UnitType)i] / 2;
    }
}

void ArmyManager::initializeArmyForBattle()
{
    // 这里可以根据需要初始化兵力，比如从存档或默认值
    // 目前假设已经在其他地方设置好了兵力
    for (int i = 0; i < 4; i++)
    {
        _armyPool[(UnitType)i] = _armyPool[(UnitType)i] * 2;
    }
}

void ArmyManager::setTotalCapacity(int capacity)
{
    _totalCapacity = capacity;
}

int ArmyManager::getTotalCapacity() const
{
    return _totalCapacity;
}

void ArmyManager::setUsedCapacity(int capacity)
{
    _usedCapacity = capacity;
}

int ArmyManager::getUsedCapacity() const {
    int used = 0;
    for (const auto& [type, count] : _armyPool) {
        int cost = 1;
        if (type == UnitType::TANK)
            cost = 5; // 简化硬编码

        else if (type == UnitType::WALL_BREAKER)
            cost = 2;

        else if (type == UnitType::RANGED)
            cost = 1;

        else if (type == UnitType::MELEE)
            cost = 1;

        else
            cost = 0;

        used += count * cost;
    }
    return used;
}