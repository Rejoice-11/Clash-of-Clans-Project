#pragma once

#include "cocos2d.h"
#include "Classes/Core/GameDirector.h"
#include "Classes/Scene/BattleScene.h"
#include "Classes/System/ResourceManager.h"
#include "Classes/UI/HUDLayer.h"
// 引入商店弹窗头文件
#include "Classes/UI/StoreWindow.h"
#include "Classes/Utils/GridUtils.h"
#include "Classes/UI/BuildingPanel.h"
// 引入设置层
#include "Classes/Setting/SettingLayer.h"
USING_NS_CC;

class VillageScene : public Scene
{
public:
    static Scene* createScene();
    virtual bool init() override;
    virtual void update(float dt) override;
    virtual void onEnter() override; // 添加进入场景回调
    virtual void onExit() override;  // 添加退出场景回调
    // 按钮回调声明
    void onAttackButtonClicked(Ref* sender);
    void onShopButtonClicked(Ref* sender);
    void onBuilderButtonClicked(Ref* sender);

    CREATE_FUNC(VillageScene);

private:
    Layer* _backgroundLayer;
    Layer* _buildingLayer;
    Layer* _uiLayer;
    LayerColor* _grayMask = nullptr;
    Node* _attackPanel = nullptr;

    // 新增：商店弹窗实例（替代原_marketPanel）
    StoreWindow* _storeWindow = nullptr;

    // 内部回调
    void closeAttackPanel(Ref* sender);
    void onMarketButtonClicked(Ref* sender); // 仅保留按钮点击入口
    void onStoreWindowClosed(); // 商店窗口关闭后的回调

    // 缩放拖动相关变量（无变更）
    Node* _scrollNode;
    Sprite* _backgroundSprite;
    Vec2 _lastTouchPos;
    bool _isDragging;
    float _scaleMin;
    float _scaleMax;
    Size _backgroundSize;
    Size _visibleSize;

    // 缩放拖动相关函数（无变更）
    void initScrollNode();
    void registerMouseEvents();
    void onMouseScroll(EventMouse* event);
    void onMouseDown(EventMouse* event);
    void onMouseMove(EventMouse* event);
    void onMouseUp(EventMouse* event);
    void clampScrollNodePosition();
    void clampScrollNodeScale(float targetScale);

	// 新增：建筑放置相关变量和函数
    StoreWindow::BuildingType _pendingBuildingType = StoreWindow::BuildingType::MAX_TYPES;  // 当前待放置类型
    Sprite* _ghostBuilding = nullptr;              // 幽灵建筑（半透明跟随鼠标）
    bool _isPlacementMode = false;                 // 是否在放置模式
	bool _isInMarketPanel = false;                     // 是否在商店界面
	bool _isInAttackPanel = false;                // 是否在攻击面板

    void enterPlacementMode(StoreWindow::BuildingType type);   // 进入放置
    void cancelPlacementMode();                                // 取消放置
    void confirmPlacement(const Vec2& worldPos);               // 确认放置
    void updateGhostPosition(const Vec2& mousePos);            // 更新幽灵位置
    std::string getGhostSpriteName(StoreWindow::BuildingType type);  // 根据类型拿图

	// 新增: 存储与gold,elixir相关函数
    void onStorageUpgraded(StorageBuilding * storage); // 响应升级
    void recalculateMaxStorage();                    // 重新计算总容量

	// 新增: 存储与gold,elixir相关建筑列表

    // 所有建筑的容器（按类型分类）
    std::vector<std::unique_ptr<TownHall>> _townHalls;
    std::vector<std::unique_ptr<ResourceBuilding>> _goldMines;
    std::vector<std::unique_ptr<ResourceBuilding>> _elixirCollectors;
    std::vector<std::unique_ptr<StorageBuilding>> _goldStorages;
    std::vector<std::unique_ptr<StorageBuilding>> _elixirStorages;
    std::vector<std::unique_ptr<DefenseBuilding>> _archerTowers;
    std::vector<std::unique_ptr<DefenseBuilding>> _cannons;

    // 当前选中的建筑
    Sprite* _selectedBuildingSprite = nullptr;
    Building* _selectedBuilding = nullptr;
    StoreWindow::BuildingType _selectedBuildingType = StoreWindow::BuildingType::MAX_TYPES;

    // 建筑操作面板
    Node* _buildingActionPanel = nullptr;
    bool _isInBuildingActionMode = false;

    // 辅助函数
    StoreWindow::BuildingType getBuildingTypeFromSprite(Sprite* sprite);
    void onBuildingClicked(Sprite* sprite);
    void showBuildingActionPanel();
    void hideBuildingActionPanel();
    void onInfoButtonClicked(Ref* sender);
    void onMoveButtonClicked(Ref* sender);
    void onBuildingPanelClosed();
    void hideBuildingActionPanelForInfoPanel();

    // 根据类型获取建筑容器
    std::vector<std::unique_ptr<Building>>* getBuildingContainer(StoreWindow::BuildingType type);
    Building* findBuildingBySprite(Sprite* sprite);

    // ... 其他成员 ...

    bool _isMovingBuilding = false;          // 新增：是否在移动建筑
    Building* _movingBuilding = nullptr;     // 要移动的建筑对象
    cocos2d::Sprite* _movingSprite = nullptr; // 原精灵（用于后续恢复）
    // 音频相关成员变量（简化为使用SettingLayer）
    MenuItemSprite* _settingButton = nullptr;        // 设置按钮
    SettingLayer* _settingLayer = nullptr;          // 设置层

    // 音频相关函数
    void addSettingButton();                        // 添加设置按钮
    void onSettingButtonClicked(Ref* sender);       // 设置按钮点击回调
    void exitGame();                                // 退出游戏
};