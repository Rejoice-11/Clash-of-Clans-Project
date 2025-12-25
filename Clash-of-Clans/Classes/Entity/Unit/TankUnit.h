//巨人（高血量+防御优先）
#pragma once

#include "Classes/Entity/Unit/Unit.h"
#include "Classes/Utils/CameraUtils.h" 
/**
 * @brief 巨人/坦克兵种类
 * 特点：高血量、低攻速（数值由 UnitData 控制）、仅针对建筑
 */
class TankUnit : public Unit
{
public:
    // 使用父类构造函数
    using Unit::Unit;

    // 实现攻击逻辑（近战）
    void attack(GameObject* target) override;

    // 返回坦克类别，用于 AI 系统识别
    UnitClass getUnitClass() const override { return UnitClass::TANK; }

    // 重写攻击偏好：巨人只会锁定建筑（基类默认已实现，此处可显式保留或扩展）
    bool prefersTarget(const Building* target) const override;

    // 渲染相关：创建巨人的 Sprite
    cocos2d::Sprite* createSprite() override;
};