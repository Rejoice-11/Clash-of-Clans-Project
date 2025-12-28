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

void ArmyManager::setArmyPool(const std::map<UnitType, int>& newPool)
{
    _armyPool = newPool;
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

int ArmyManager::getUsedCapacity() const 
{
    
    return _usedCapacity;
}