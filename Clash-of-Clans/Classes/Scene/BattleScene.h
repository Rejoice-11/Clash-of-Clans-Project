#pragma once

#include "cocos2d.h"
#include "Classes/Core/GameDirector.h"
#include "Classes/Utils/GridUtils.h"
USING_NS_CC;

class BattleScene : public Scene
{
public:
	// 鼠标事件注册及回调声明
    void registerMouseEvents2();
    void onMouseScroll2(EventMouse* event);
    void onMouseDown2(EventMouse* event);
    void onMouseMove2(EventMouse* event);
    void onMouseUp2(EventMouse* event);
    //士兵生成
	void spawnSoldierAtPosition(const Vec2& position);
    // 函数声明（仅写原型，不写实现）
    static Scene* createScene();
    virtual bool init() override;
    virtual void update(float dt) override;  // 战斗实时模拟核心

    // 按钮回调声明
    void onSurrenderButtonClicked(Ref* sender);  // 投降
    void onNextWaveButtonClicked(Ref* sender);   // 预留下一波
	void onReturnButtonClicked(Ref* sender);    // 返回村庄
	void onSoldierSelectButtonClicked(Ref* sender); // 选择士兵（未实现）
	void closeReturnWindow(Ref* sender);  // 关闭结束战斗确认窗口(未实现)
	void closeBattleScene(Ref* sender);        // 关闭战斗场景(回到villagescene)(未实现)
    MenuItemImage* BattleScene::createSoldierButton(const Vec2& btnPos, const std::string& smallIconPath);// 封装：创建带小图标的士兵按钮（避免重复代码）
    // Cocos2d-x 创建实例宏
    CREATE_FUNC(BattleScene);

private:
    // 成员变量声明
    Layer* _backgroundLayer;   // 敌方村庄背景
    Layer* _buildingLayer;     // 敌方建筑
    Layer* _unitLayer;         // 玩家部队 + 子弹
    Layer* _effectLayer;       // 爆炸特效等
    Layer* _uiLayer;           // 战斗HUD（剩余部队、时间、星级）
	bool _isBattleStart=false;        // 战斗是否开始
	bool _isBattleOver = false;         // 战斗是否结束
    // 新增：保存弹窗面板和菜单（用于后续删除）
    cocos2d::Sprite* _returnPanel = nullptr;
    cocos2d::Menu* _returnMenu = nullptr;
};