//野蛮人（近战）
#pragma once

#include "Classes/Entity/Unit/Unit.h"

/**
 * @brief 野蛮人/近战士兵类
 */
class MeleeUnit : public Unit
{
public:
    // 使用父类构造函数
    using Unit::Unit;

    // 实现基类纯虚函数
    UnitClass getUnitClass() const override { return UnitClass::MELEE; }

    // 渲染相关：创建野蛮人的 Sprite
    cocos2d::Sprite* createSprite() override;

    // 如果野蛮人有特殊逻辑（比如狂暴），可以重写 attack
    void attack(GameObject* target) override;
};