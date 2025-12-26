// BuildingPanel.h
#pragma once
#include "cocos2d.h"
#include "Classes/Entity/Building/Building.h"

class BuildingPanel : public cocos2d::Layer {
public:
    static BuildingPanel* create(Building* building, const std::function<void()>& onClose);
    bool init(Building* building, const std::function<void()>& onClose);

private:
    void onUpdateButtonClicked(Ref* sender);
    void onCloseButtonClicked(Ref* sender);

    // 新增：刷新面板数据（用于升级后）
    void refreshPanel();

    Building* _building;
    std::function<void()> _onCloseCallback;

    // UI 元素指针（用于刷新）
    cocos2d::Sprite* _buildingImage = nullptr;
    cocos2d::Label* _levelLabel = nullptr;
    cocos2d::Label* _costLabel = nullptr;
    std::vector<cocos2d::Label*> _statLabels; // 存储所有属性数值标签
};