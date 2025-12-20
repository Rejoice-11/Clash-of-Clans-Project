//大本营（特殊胜利条件）
#pragma

#include "Classes/Entity/Building/Building.h"
#include "cocos2d.h"

/**
 * @brief 大本营（Town Hall）——村庄核心建筑
 * @note
 *   - 摧毁它 = 获得1颗星（战斗评分）
 *   - 希望"不同等级使用不同精灵帧（town_hall_lv1.png ~ lv3.png）"但现在先全部用同一个
 *   - 非防御建筑（巨人不会优先攻击它）
 */
 // 声明全局变量（不定义！）
extern int countofTownHallsInVillage; // 全局计数器
extern BuildingData TownHallBuildingData; // 配置数据

class TownHall : public Building 
{
public:

	//track number of Town Halls in the village

    /**
     * @brief 构造函数
     * @param data 配置数据（来自JSON或Constants）
     * @param instanceId 实例唯一ID（用于存档/网络）
     */
    explicit TownHall(const BuildingData& data, int instanceId = -1);

    // ———————— 重写基类接口 ———————— //

    cocos2d::Sprite* createSprite() override;

    bool isDefenseStructure() const override { return false; }

    void upgrade() override;

    // ———————— 大本营特有逻辑 ———————— //

    /**
     * @brief 检查是否被摧毁（HP <= 0）
     * @note 战斗系统应监听此状态变化以触发"获得1星"
     */
    bool isDestroyed() const { return getState() == State::DESTROYED; }

    /**
     * @brief 获取当前等级对应的精灵名称
     * @example "town_hall_lv2"
     */
	std::string getSpriteFrameName() const;//for the information page and for changing the sprite when upgrading

private:
    // 禁止拷贝（资源管理安全）
    TownHall(const TownHall&) = delete;
    TownHall& operator=(const TownHall&) = delete;
};
