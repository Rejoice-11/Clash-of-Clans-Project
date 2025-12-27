#pragma once
#include "cocos2d.h"
#include "Classes/Entity/Building/TownHall.h"  // 暂时先包含TownHall，其他建筑后续加
#include "Classes/Entity/Building/ResourceBuilding.h"
#include "Classes/Entity/Building/StorageBuilding.h"
#include "Classes/Entity/Building/MilitaryBuilding.h"
#include "Classes/Entity/Building/DefenseBuilding.h"
#include <functional>

USING_NS_CC;

class StoreWindow : public Layer
{
public:
    // 建筑类型枚举（方便扩展）
    enum class BuildingType
    {
        TOWN_HALL = 0,
        GOLD_MINE = 1,
        ELIXIR_COLLECTOR = 2,
        GOLD_STORAGE = 3,
        ELIXIR_STORAGE = 4,
        MILITARY_CAMP = 5,
        ARCHER_TOWER = 6,
        CANNON = 7,
        WORKER_HOME = 8,
        MAX_TYPES = 9  // 计数用
    };

    static StoreWindow* create(const std::function<void(BuildingType)>& placeCallback);  // 改！回调带类型

    bool initWithPlaceCallback(const std::function<void(BuildingType)>& placeCallback);

    void rebuildMenu();

    void show();
    void hide();

    Menu* _menu;

private:
    void onCloseButtonClicked(Ref* sender);

    Size _visibleSize;
    Vec2 _origin;

    LayerColor* _grayMask = nullptr;
    Sprite* _storePanel = nullptr;

    std::function<void(BuildingType)> _placeCallback;  // 关闭时告诉Village要放置哪个

    // 当前大本等级（假设从全局拿，暂时写死测试用1级）
    int _currentTownHallLevel = 1;  // 以后从ResourceManager或存档拿

    // 每个建筑当前数量（从全局计数拿）
    int getCurrentCount(BuildingType type);
    int getMaxCount(BuildingType type, int thLevel);
};
