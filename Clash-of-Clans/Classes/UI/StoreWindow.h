#pragma once

#include "cocos2d.h"
#include "Classes/Core/ConfigManager.h"
#include "Classes/System/ResourceManager.h"

USING_NS_CC;

class StoreWindow : public Layer
{
public:
    virtual bool init() override;

    void show();
    void hide();

    // 点击建筑卡片与关闭按钮的回调
    void onBuildingCardClicked(Ref* sender);
    void onCloseButtonClicked(Ref* sender);

    CREATE_FUNC(StoreWindow);

private:
    Sprite* _panel = nullptr;
    LayerColor* _backgroundMask = nullptr;
    std::vector<int> _availableBuildings;
};
