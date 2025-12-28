// 防御建筑（箭塔 + 加农炮）
#pragma once

#include "Classes/Entity/Building/Building.h"
#include "cocos2d.h"

extern int countofArcherTowersInVillage; // 箭塔全局计数
extern int countofCanonsInVillage;       // 加农炮全局计数

extern BuildingData ArcherTowerBuildingData; // 建筑数据
extern BuildingData CanonBuildingData;

/**
 * @brief 防御建筑（箭塔、加农炮）
 * @note 可攻击敌人，有射程和伤害
 */
class DefenseBuilding : public Building 
{
    public:
        enum class DefenseType 
        {
            ARCHER_TOWER,
            CANON
        };
    
        explicit DefenseBuilding(const BuildingData& data, int instanceId = -1, DefenseType type = DefenseType::ARCHER_TOWER);
    
        // ———————— 重写基类 ———————— //
        cocos2d::Sprite* createSprite() override;

        bool isDefenseStructure() const override 
        { 
            return true;
        }

        void upgrade() override;
    
        // ———————— 防御特有接口 ———————— //
        DefenseType getDefenseType() const { return _defenseType; }
    
        /**
         * @brief 获取当前等级对应的精灵名称
         * @example "archer_tower_lv2", "canon_lv3"
         */
        std::string getSpriteFrameName() const;
    
        /**
         * @brief 获取当前等级的攻击范围（像素）
         */
        float getAttackRange() const;
    
        /**
         * @brief 获取当前等级的每秒伤害（DPS）
         */
        float getDamagePerSecond() const;
    
    private:
        DefenseType _defenseType;
};