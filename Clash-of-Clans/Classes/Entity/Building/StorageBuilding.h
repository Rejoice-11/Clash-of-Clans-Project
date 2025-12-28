//储金罐/圣水瓶
// 储金罐/圣水瓶
#pragma once

#include "Classes/Entity/Building/Building.h"
#include "cocos2d.h"

extern int countofGoldStoragesInVillage;    // 全局计数
extern int countofElixirStoragesInVillage;  // 全局计数

extern BuildingData GoldStorageBuildingData;    // 建筑数据
extern BuildingData ElixirStorageBuildingData;  // 建筑数据

/**
 * @brief 资源存储建筑（储金罐、圣水瓶）
 * @note 提供资源容量，不产出资源
 */
class StorageBuilding : public Building
{
    public:
        enum class StorageType 
        {
            GOLD_STORAGE,
            ELIXIR_STORAGE
        };
    
        explicit StorageBuilding(const BuildingData& data, int instanceId = -1, StorageType type = StorageType::GOLD_STORAGE);
    
        // ———————— 重写基类 ———————— //
        cocos2d::Sprite* createSprite() override;

        bool isDefenseStructure() const override
        { 
            return false; 
        }
        void upgrade() override;
    
        // ———————— 存储特有接口 ———————— //
        StorageType getStorageType() const 
        { 
            return _storageType;
        }
    
        /**
         * @brief 获取当前等级对应的精灵名称
         * @example "gold_storage_lv2"
         */
        std::string getSpriteFrameName() const;
    
        /**
         * @brief 获取当前等级的存储容量
         * @return 容量值（如 5000）
         */
        int getCapacity() const;
    
    private:
        StorageType _storageType;
};