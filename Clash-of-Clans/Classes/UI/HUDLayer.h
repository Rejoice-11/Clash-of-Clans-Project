//顶部资源栏（金币/圣水/工人）
#pragma once
#include "cocos2d.h"

class HUDLayer : public cocos2d::Layer {
public:
    static HUDLayer* create();
    virtual bool init() override;

private:
    void onResourceChanged();
    void updateDisplay();

    // === 金币 UI 元素 ===
    cocos2d::Sprite* _goldIcon = nullptr;
    cocos2d::Label* _goldLabel = nullptr;
    cocos2d::Sprite* _goldFillBar = nullptr;
    cocos2d::Sprite* _goldBackgroundBar = nullptr;

    // === 圣水 UI 元素 ===
    cocos2d::Sprite* _elixirIcon = nullptr;
    cocos2d::Label* _elixirLabel = nullptr;
    cocos2d::Sprite* _elixirFillBar = nullptr;
    cocos2d::Sprite* _elixirBackgroundBar = nullptr;

    const float BAR_WIDTH = 120.0f;
    const float BAR_HEIGHT = 12.0f;
    const float ICON_SIZE = 40.0f;
};


