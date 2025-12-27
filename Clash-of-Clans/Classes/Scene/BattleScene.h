#pragma once

#include "cocos2d.h"
#include "Classes/Core/GameDirector.h"
#include "Classes/Utils/GridUtils.h"
#include "Classes/UI/BuildingPanel.h"
#include "Classes/Entity/Building/StorageBuilding.h"
#include "Classes/Entity/Building/MilitaryBuilding.h"
#include "Classes/Entity/Building/DefenseBuilding.h"
#include "Classes/Entity/Building/Building.h"
#include "Classes/Entity/Building/ResourceBuilding.h"
#include "Classes/Entity/Building/TownHall.h"
#include "Classes/Utils/Constants.h"
#include "Classes/Data/UnitData.h"
#include "Classes/Core/ConfigManagerUnit.h"
#include "SimpleAudioEngine.h"
#include <map>

USING_NS_CC;
using namespace CocosDenshion;

class BattleScene : public Scene
{
public:
    //战斗结束时候调用
	void battleOver(bool isWin);
	// 建筑生成
    void spawnBuilding(Vec2 gridPos,int Buildingname);
	// 鼠标事件注册及回调声明
    void registerMouseEvents2();
    void onMouseScroll2(EventMouse* event);
    void onMouseDown2(EventMouse* event);
    void onMouseMove2(EventMouse* event);
    void onMouseUp2(EventMouse* event);
    //士兵生成
	void spawnSoldierAtPosition(const Vec2& position);
    void updateArrowPosition(cocos2d::MenuItemImage* targetBtn); // 新增：更新箭头位置
    // 函数声明（仅写原型，不写实现）
    static Scene* createScene();
    virtual bool init() override;
    virtual void update(float dt) override;  // 战斗实时模拟核心
	void initgridinfo(const int x,const int y,int buildingtype); // 初始化网格信息
    // 按钮回调声明
    void onSurrenderButtonClicked(Ref* sender);  // 投降
    void onNextWaveButtonClicked(Ref* sender);   // 预留下一波
	void onReturnButtonClicked(Ref* sender);    // 返回村庄
	void onSoldierSelectButtonClicked(Ref* sender); // 选择士兵（未实现）
	void closeReturnWindow(Ref* sender);  // 关闭结束战斗确认窗口(未实现)
	void closeBattleScene(Ref* sender);        // 关闭战斗场景(回到villagescene)(未实现)
    MenuItemImage* BattleScene::createSoldierButton(const Vec2& btnPos, const std::string& smallIconPath,UnitType type);// 封装：创建带小图标的士兵按钮（避免重复代码）
    // Cocos2d-x 创建实例宏
    CREATE_FUNC(BattleScene);

    //UI刷新函数
    void refreshUI();

private:
	cocos2d::Sprite* _arrowIndicator = nullptr; // 新增：箭头指示器
	cocos2d::MenuItemImage* _selectedSoldierBtn = nullptr; // 新增：记录当前选中的士兵按钮
    UnitType _selectedType = UnitType::NONE; // 当前选中的兵种

    // UI 映射表：通过兵种类型直接找到对应的 按钮 和 数字标签
    std::map<UnitType, cocos2d::MenuItemImage*> _unitButtons;
    std::map<UnitType, cocos2d::Label*> _unitLabels;


    // 成员变量声明
    Layer* _backgroundLayer;   // 敌方村庄背景
    Layer* _buildingLayer;     // 敌方建筑
    Layer* _unitLayer;         // 玩家部队 + 子弹
    Layer* _effectLayer;       // 爆炸特效等
    Layer* _uiLayer;           // 战斗HUD（剩余部队、时间、星级）
	bool _isBattleStart=false;        // 战斗是否开始
	bool _isBattleOver = false;         // 战斗是否结束
	bool _isTownHallDestroyed = false; // 大本营是否被摧毁
    // 新增：保存弹窗面板和菜单（用于后续删除）
    cocos2d::Sprite* _returnPanel = nullptr;
    cocos2d::Menu* _returnMenu = nullptr;

    cocos2d::Sprite* realSprite = nullptr;    // 创建建筑对象便于生成建筑
    /* 所有建筑的容器（按类型分类）
    std::vector<std::unique_ptr<TownHall>> _townHalls;
    std::vector<std::unique_ptr<ResourceBuilding>> _goldMines;
    std::vector<std::unique_ptr<ResourceBuilding>> _elixirCollectors;
    std::vector<std::unique_ptr<StorageBuilding>> _goldStorages;
    std::vector<std::unique_ptr<StorageBuilding>> _elixirStorages;
    std::vector<std::unique_ptr<DefenseBuilding>> _archerTowers;
    std::vector<std::unique_ptr<DefenseBuilding>> _cannons;
    */
};