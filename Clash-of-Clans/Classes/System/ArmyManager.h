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

    /** @brief 获取军队池（用于修改） */
    std::map<UnitType, int>& getArmyPool() { return _armyPool; }

    const std::map<UnitType, int>& getArmyPool() const { return _armyPool; } // ← 新增 getter

    void setTotalCapacity(int capacity);

    /** @brief 获取当前总兵力上限 */
    int getTotalCapacity() const;

    /** @brief 计算已使用的兵力 */
    int getUsedCapacity() const;

    void setUsedCapacity(int capacity);
    //计算剩余兵力
    void getRemainingCapacity();
    // 计算进入时有的兵力
    void initializeArmyForBattle();
private:
    ArmyManager() = default;
    static ArmyManager* _instance;
    std::map<UnitType, int> _armyPool;
    int _totalCapacity = 0; //  新增成员
    int _usedCapacity = 0;  //  新增成员
};