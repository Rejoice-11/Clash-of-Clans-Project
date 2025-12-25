//弓箭手（远程）
#pragma once

#include "Unit.h"

/**
 * @brief 远程单位（如弓箭手 Archer）
 */
class RangedUnit : public Unit
{
public:
    using Unit::Unit; // 继承构造函数

    UnitClass getUnitClass() const override { return UnitClass::RANGED; }

    // 渲染：创建弓箭手的 Sprite
    cocos2d::Sprite* createSprite() override;

    // 重写攻击：远程单位不需要贴身
    void attack(GameObject* target) override;

private:
    // 简单的“射击”表现逻辑
    void fireProjectile(GameObject* target);
};