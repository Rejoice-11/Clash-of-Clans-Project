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

    // 常量保持不变
    const float ICON_SIZE = 40.0f;    // 图标尺寸保持不变
    const float BAR_WIDTH = 200.0f;   // 进度条宽度
    const float BAR_HEIGHT = 30.0f;   // 进度条高度
};


