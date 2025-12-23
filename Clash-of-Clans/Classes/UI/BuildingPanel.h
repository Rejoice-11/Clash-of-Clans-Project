//建筑信息弹窗（信息/升级/移动）
#pragma once
#include "cocos2d.h"
#include "Classes/Entity/Building/Building.h"
#include "Classes/Core/GameDirector.h"
#include "Classes/Entity/Building/TownHall.h"
#include "Classes/Entity/Building/ResourceBuilding.h"

class BuildingPanel : public cocos2d::Layer 
{
public:
    static BuildingPanel* create(Building* building, const std::function<void()>& onClose);
    bool init(Building* building, const std::function<void()>& onClose);

private:
    void onUpdateButtonClicked(Ref* sender);
    void onCloseButtonClicked(Ref* sender);

    Building* _building;
    std::function<void()> _onCloseCallback;
    cocos2d::Label* _levelLabel;
    cocos2d::Label* _costLabel;
};