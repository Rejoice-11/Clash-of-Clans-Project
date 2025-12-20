#pragma once

#include "cocos2d.h"
#include "Classes/Core/GameDirector.h"

USING_NS_CC;

class BattleScene : public Scene
{
public:
    // 函数声明（仅写原型，不写实现）
    static Scene* createScene();
    virtual bool init() override;
    virtual void update(float dt) override;  // 战斗实时模拟核心

    // 按钮回调声明
    void onSurrenderButtonClicked(Ref* sender);  // 投降
    void onNextWaveButtonClicked(Ref* sender);   // 预留下一波

    // Cocos2d-x 创建实例宏
    CREATE_FUNC(BattleScene);

private:
    // 成员变量声明
    Layer* _backgroundLayer;   // 敌方村庄背景
    Layer* _buildingLayer;     // 敌方建筑
    Layer* _unitLayer;         // 玩家部队 + 子弹
    Layer* _effectLayer;       // 爆炸特效等
    Layer* _uiLayer;           // 战斗HUD（剩余部队、时间、星级）
};