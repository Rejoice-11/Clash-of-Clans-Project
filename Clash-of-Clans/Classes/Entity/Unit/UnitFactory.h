#pragma once

#include "Unit.h"
#include "MeleeUnit.h"
#include "TankUnit.h"
#include "RangedUnit.h"
#include "WallBreaker.h"
#include "Classes/Core/ConfigManagerUnit.h" 

/**
 * @brief 士兵生产工厂
 */
class UnitFactory
{
    public:
        /**
         * @brief 根据兵种类型创建具体实例
         */
        static Unit* createUnit(UnitType type) 
        {
            // 1. 从配置中心拿到图纸 (UnitData)
            // 这里强转 int 是为了匹配 JSON 里的 ID 键值
            const UnitData& data = ConfigManagerUnit::getInstance()->getUnitData(static_cast<int>(type));
    
            Unit* unit = nullptr;
    
            // 2. 根据枚举类型，决定实例化哪个子类
            switch (type) {
                case UnitType::MELEE:
                    unit = new (std::nothrow) MeleeUnit(data);
                    break;

                case UnitType::TANK:
                    unit = new (std::nothrow) TankUnit(data);
                    break;

                case UnitType::RANGED:
                    unit = new (std::nothrow) RangedUnit(data);
                    break;

                case UnitType::WALL_BREAKER:
                    unit = new (std::nothrow) WallBreaker(data);
                    break;
    
                default:
                    CCLOG("UnitFactory: Unknown UnitType %d", static_cast<int>(type));
                    break;
            }
    
            // 3. Cocos2d-x 内存管理：加入自动释放池
            if (unit) 
            {
                unit->autorelease();
            }
    
            return unit;
        }
    
    private:
        // 静态工厂不需要构造函数
        UnitFactory() = default;
};