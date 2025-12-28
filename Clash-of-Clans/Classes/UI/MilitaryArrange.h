// MilitaryArrange.h
#pragma once
#include "cocos2d.h"
#include <Data/UnitData.h>
#include <2d/CCMenuItem.h>

USING_NS_CC;

class MilitaryArrange : public cocos2d::Layer
{
public:
    // 标准无参 create（和 StoreWindow 一模一样！）
    static MilitaryArrange* create();

    // 初始化函数（无参！）
    bool init();

    // 设置回调（仿 StoreWindow 的 setPlaceCallback）

    struct TroopButton
    {
        MenuItemSprite* button;
        UnitType type;
        int cost;
    };

    void setCloseCallback(const std::function<void()>& callback);

private:
    std::function<void()> _closeCallback = nullptr;

    // UI 元素（和之前一样）
    Sprite* _background = nullptr;
    Node* _troopBar = nullptr;
    std::vector<std::pair<Sprite*, Label*>> _troopDisplays;

    std::vector<TroopButton> _troopButtons;

    Label* _capacityLabel = nullptr;

    MilitaryArrange() = default; // 私有构造

    void refreshTroopBar();
    void onTroopButtonClicked(TroopButton* btn);
    void onCloseButtonClicked(Ref* sender);
    void updateButtonStates();
};