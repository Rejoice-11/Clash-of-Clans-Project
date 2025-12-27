// MilitaryArrang.h
#pragma once
#include "cocos2d.h"
#include "Classes/System/ArmyManager.h"
#include "Classes/Scene/VillageScene.h"
/*
class MilitaryArrang : public cocos2d::Layer {
public:
    static MilitaryArrang* create(VillageScene* villageScene);

    bool init(VillageScene* villageScene);

private:
    VillageScene* _villageScene = nullptr;
    Sprite* _background = nullptr;
    Node* _troopBar = nullptr; // 已配置军队显示区
    std::vector<std::pair<Sprite*, Label*>> _troopDisplays; // <图标, 数量标签>

    // 兵种按钮和数据
    struct TroopButton {
        MenuItemSprite* button;
        UnitType type;
        int cost; // 每个单位消耗的兵力
    };
    std::vector<TroopButton> _troopButtons;

    Label* _capacityLabel = nullptr; // 总兵力显示

    void refreshTroopBar();      // 刷新已配置军队显示
    void onTroopButtonClicked(TroopButton* btn);
    void onCloseButtonClicked(Ref* sender);
    void updateButtonStates();   // 更新按钮灰显状态

    CREATE_FUNC_PARAM(MilitaryArrang, VillageScene*, villageScene);
};*/