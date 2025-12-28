//金矿/圣水收集器
#pragma once
#include "Classes/Entity/Building/Building.h"
#include "Classes/Data/BuildingData.h"
#include "cocos2d.h"

extern int countofGoldMinesInVillage; // 全局计数器
extern int countofElixirCollectorsInVillage; // 全局计数器1
extern BuildingData GoldMineBuildingData; // 配置数据
extern BuildingData ElixirCollectorBuildingData; // 配置数据
/**
 * @brief 资源生产建筑基类（金矿、圣水收集器）
 * @note 自动周期性产出资源到对应储罐
 */
class ResourceBuilding : public Building
{
    public:
        enum class ResourceType
        {
            GOLD,
            ELIXIR
        };
    
        explicit ResourceBuilding(const BuildingData& data, int instanceId = -1, ResourceType type = ResourceType::GOLD);
    
        // ———————— 重写基类 ———————— //
    
        cocos2d::Sprite* createSprite() override;
    
        bool isDefenseStructure() const override 
        { 
            return false;
        
        }
    
        void upgrade() override;
    
        // ———————— 资源特有接口 ———————— //
        ResourceType getResourceType() const 
        { 
            return _resourceType; 
        }
    
        /**
         * @brief 获取当前等级对应的精灵名称
         * @example "town_hall_lv2"
         */
    
        std::string getSpriteFrameName() const;//for the information page and for changing the sprite when upgrading
        /**
         * @brief 每帧或定时调用，尝试产出资源
         * @param deltaTime 上一帧时间（秒）
         * @return 实际产出量（可能为0，若储罐满）
         */
        virtual int produce(float deltaTime);
    
    protected:
        ResourceType _resourceType;
        float _accumulatedTime = 0.0f; // 累计未触发的时间
};
