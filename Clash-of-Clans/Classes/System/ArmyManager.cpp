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
            cost = 5; // ¼ò»¯Ó²±àÂë

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