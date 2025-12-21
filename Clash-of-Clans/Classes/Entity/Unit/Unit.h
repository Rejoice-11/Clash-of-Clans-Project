//基类（HP、攻击、寻路）
#pragma once

#include "Classes/Entity/GameObject.h"
#include "Classes/Data/UnitData.h"
#include "Classes/Entity/Building/Building.h"

/**
 * @brief 士兵单位抽象基类
 * @note 所有兵种（野蛮人、弓箭手等）必须继承此类
 */
class Unit : public GameObject 
{
public:
    static constexpr Type TYPE = Type::UNIT;
    Type getType() const override { return TYPE; }

    explicit Unit(const UnitData& data, int instanceId = -1)
        : GameObject(instanceId), _data(data), _hp(data.hp) 
    {
    }

    // 属性访问
    int getMaxHp() const { return _data.hp; }
    int getCurrentHp() const { return _hp; }
    void setHp(int hp) { _hp = std::max(0, hp); }
    bool isDead() const { return _hp <= 0; }

    int getDamage() const { return _data.damage; }
    float getAttackRange() const { return _data.attackRange; }
    float getSpeed() const { return _data.speed; }

    // 兵种类型（用于AI决策）
    enum class UnitClass 
    {
        MELEE,
        RANGED,
        TANK,
        WALL_BREAKER,
        //AIR(when time is enough for explore)
    };
    virtual UnitClass getUnitClass() const = 0;

    // 攻击偏好（如巨人只打防御建筑）
    virtual bool prefersTarget(const Building* target) const;

    // 移动与攻击（由CombatSystem调用）
    virtual void moveTowards(const cocos2d::Vec2& target);
    virtual void attack(GameObject* target);

    // 纯虚函数：子类提供渲染精灵
    virtual cocos2d::Sprite* createSprite() = 0;

protected:
    const UnitData& _data;
    int _hp;
};