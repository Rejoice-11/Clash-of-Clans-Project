//基类（HP、攻击、寻路）
#pragma once

#include "Classes/Entity/GameObject.h"
#include "Classes/Data/UnitData.h"
#include "Classes/Entity/Building/Building.h"
#include "Classes/Scene/BattleScene.h"
#include "SimpleAudioEngine.h"
// 声明外部定义的网格，确保士兵能访问建筑信息
using namespace CocosDenshion;

/**
 * @brief 士兵单位抽象基类
 * @note 所有兵种（野蛮人、弓箭手等）必须继承此类
 */
class Unit : public GameObject 
{
    public:
        static constexpr Type TYPE = Type::UNIT;
        Type getType() const override
        {
            return TYPE;
        }
    
        explicit Unit(const UnitData& data, int instanceId = -1)
            : GameObject(instanceId), _data(data), _hp(data.hp) 
        {
        }
        // 承伤逻辑
        virtual void takeDamage(int amount); // 新增承伤接口
        virtual void update(float dt);      // 增加每帧更新接口
    
        // 属性访问
        int getMaxHp() const
        {
            return _data.hp;
        }

        int getCurrentHp() const 
        {
            return _hp;
        }

        void setHp(int hp) 
        { 
            _hp = std::max(0, hp);
        }
        bool isDead() const 
        { 
            return _hp <= 0; 
        }
    
        int getDamage() const 
        { 
            return _data.damage;
        }

        float getAttackRange() const 
        { 
            return _data.attackRange;
        }

        float getSpeed() const
        {
            return _data.speed;
        }
    
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
    
    
    	// 攻击计时器相关
        bool Unit::canAttack()
        {
            return _attackTimer >= _data.attackInterval;
        }
        
        void Unit::resetAttackTimer() 
        {
            _attackTimer = 0.0f;
        }
    
        // 移动与攻击（由CombatSystem调用）
    	virtual void moveTowards(const cocos2d::Vec2& target, float dt);
      
        // 纯虚函数：子类提供渲染精灵
        virtual cocos2d::Sprite* createSprite() = 0;
    
    protected:
        const UnitData& _data;
        int _hp;
        float _attackTimer = 0.0f; // 累计时间
        cocos2d::Sprite* _mySprite = nullptr;
    
        cocos2d::Vec2 _targetGridPos = cocos2d::Vec2(-1, -1); // 目标格点 (x, y)
    
        // 寻敌逻辑：每个子类可以有自己的搜索算法（比如巨人优先搜防御）
        virtual void findBestTarget();
    
        // 核心动作：子类重写这个函数，实现具体的攻击效果
        virtual void executeAttack(int gridX, int gridY) = 0;
};