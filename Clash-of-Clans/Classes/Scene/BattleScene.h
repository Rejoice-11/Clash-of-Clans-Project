//战斗模拟
// scene/BattleScene.h
#pragma once

#include "cocos2d.h"
USING_NS_CC;

class BattleScene : public Scene
{
public:
    static Scene* createScene();
    virtual bool init() override;
    virtual void update(float dt) override;  // 战斗实时模拟核心！！

    // 按钮回调
    void onSurrenderButtonClicked(Ref* sender);  // 投降
    void onNextWaveButtonClicked(Ref* sender);   // 预留下一波

    CREATE_FUNC(BattleScene);

private:
    Layer* _backgroundLayer;   // 敌方村庄背景
    Layer* _buildingLayer;     // 敌方建筑
    Layer* _unitLayer;         // 玩家部队 + 子弹
    Layer* _effectLayer;       // 爆炸特效等
    Layer* _uiLayer;           // 战斗HUD（剩余部队、时间、星级）

    // 你需要准备的png
    // "battle_background.png"      // 可以和村庄底图共用，或者单独一套
    // "battle_deploy_bar.png"      // 底部部队部署栏背景
    // "button_surrender_normal.png"
    // "button_surrender_pressed.png"
    // "star_empty.png" / "star_full.png"  // 星级显示用
    // "timer_icon.png"             // 战斗倒计时图标
};