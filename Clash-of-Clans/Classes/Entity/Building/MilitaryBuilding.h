//训练营/军营
// MilitaryBuilding.h
#pragma once
#include "Classes/Entity/Building/Building.h"
#include "Classes/Data/BuildingData.h"

extern BuildingData MilitaryBuildingBuildingData; // 配置数据

/**
 * @brief 军事建筑（融合兵营+军营）
 * @note 升级仅提升总兵力上限，所有兵种初始解锁
 */
class MilitaryBuilding : public Building
{
    public:
        explicit MilitaryBuilding(const BuildingData& data, int instanceId = -1);
    
        // 重写基类方法
        bool isDefenseStructure() const override 
        { 
            return false; 
        }

        void upgrade() override;
    
        /** @brief 获取当前等级的总兵力上限 */
        int getTotalTroopCapacity() const;
    
        // 创建精灵（用于 VillageScene 显示）
        cocos2d::Sprite* createSprite() override;
    
    private:
        std::string getSpriteFrameName() const;
};