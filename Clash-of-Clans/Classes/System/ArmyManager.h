//出兵控制
#pragma once
#include <map>
#include "Classes/Data/UnitData.h"

class ArmyManager {
public:
    static ArmyManager* getInstance();

    // 初始化：设定某种兵有多少个
    void setUnitCount(UnitType id, int count);

    // 获取某种兵的剩余数量
    int getRemainingCount(UnitType id);

    // 尝试部署一个（如果够扣就扣一个，并返回true）
    bool tryDeploy(UnitType id);

    const std::map<UnitType, int>& getArmyPool() const { return _armyPool; } // ← 新增 getter

private:
    ArmyManager() = default;
    static ArmyManager* _instance;
    std::map<UnitType, int> _armyPool;
};