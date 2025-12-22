//顶部资源栏（金币/圣水/工人）
#pragma once
#include "cocos2d.h"
#include "Classes/System/ResourceManager.h"
#include "Classes/Core/GameDirector.h"

class HUDLayer : public cocos2d::Layer 
{
public:
    static HUDLayer* create();
    virtual bool init() override;

private:
    void onResourceChanged(); // ResourceManager 回调
    void updateResourceLabels(); // 刷新文本

    cocos2d::Label* _goldLabel;
    cocos2d::Label* _elixirLabel;
};


