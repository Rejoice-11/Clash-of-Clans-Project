//抽象基类（纯虚函数：onUpgrade, onBuild）
#pragma once

#include "Classes/Entity/GameObject.h"
#include "Classes/Data/BuildingData.h" 
#include "cocos2d.h"
#include "Classes/Utils/GridUtils.h"

/**
 * @brief 建筑抽象基类
 * @note 所有具体建筑（大本营、炮塔等）必须继承此类
 */
class Building : public GameObject 
{
public:
    static constexpr Type TYPE = Type::BUILDING;
    Type getType() const override { return TYPE; }

    Vec2 getGridPosition() const { return _gridPos; }
    void setGridPosition(const Vec2 & pos) { _gridPos = pos; }

    // 构造时绑定配置数据
    explicit Building(const BuildingData& data, int instanceId = -1)
        : GameObject(instanceId), _data(data), _currentLevel(1) {
    }

    // 获取建筑类型ID（对应配置表）
    int getBuildingTypeId() const { return _data.id; }

    // 获取建筑类型TYPE（对应配置表）
	int getBuildingType() const { return static_cast<int>(_data.type); }

    // 获取建筑类型在此等级最大可建造数
	int getBuildingMaxCount() const { return _data.maxBuildCount[_currentLevel]; }

    // 等级系统
    int getCurrentLevel() const { return _currentLevel ; }
    virtual void upgrade(); // 升级逻辑由子类或Manager处理

    // 资源消耗（建造/升级）
    int getBuildCostGold() const { return _data.goldCost[_currentLevel]; }
    int getBuildCostElixir() const { return _data.elixirCost[_currentLevel]; }

    // 是否可被攻击（城墙通常不可被选为目标）
    virtual bool isAttackable() const { return true; }

    // 是否为防御建筑（影响巨人AI）
    virtual bool isDefenseStructure() const { return false; }

    // 纯虚函数：子类必须提供渲染节点（供Scene管理）
    virtual cocos2d::Sprite* createSprite() = 0;

protected:

    const BuildingData& _data;            // 配置数据（只读）
    int _currentLevel;              // 当前等级（1~max）
	Vec2 _gridPos = Vec2::ZERO;     // 网格坐标

};