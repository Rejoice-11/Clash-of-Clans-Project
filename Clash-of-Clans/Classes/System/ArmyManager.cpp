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