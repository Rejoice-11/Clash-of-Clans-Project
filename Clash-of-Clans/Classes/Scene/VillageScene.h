#pragma once

#include "cocos2d.h"
#include "Classes/Core/GameDirector.h"
#include "Classes/Scene/BattleScene.h"
// 引入商店弹窗头文件
#include "Classes/UI/StoreWindow.h"

USING_NS_CC;

class VillageScene : public Scene
{
public:
    static Scene* createScene();
    virtual bool init() override;
    virtual void update(float dt) override;

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

    void enterPlacementMode(StoreWindow::BuildingType type);   // 进入放置
    void cancelPlacementMode();                                // 取消放置
    void confirmPlacement(const Vec2& worldPos);               // 确认放置
    void updateGhostPosition(const Vec2& mousePos);            // 更新幽灵位置
    std::string getGhostSpriteName(StoreWindow::BuildingType type);  // 根据类型拿图
};