// 炸弹人（只打建筑，自爆）
#pragma once

#include "Unit.h"

/**
 * @brief 炸弹人单位
 * @note 特性：优先攻击墙壁（或建筑），攻击方式为走到目标面前自爆，造成范围伤害并死亡。
 */
class WallBreaker : public Unit
{
public:
    // 使用父类构造函数
    using Unit::Unit;


    // 类型标识
    UnitClass getUnitClass() const override { return UnitClass::WALL_BREAKER; }

    void WallBreaker::executeAttack(int x, int y) override;

    // 渲染相关
    cocos2d::Sprite* createSprite() override;
};