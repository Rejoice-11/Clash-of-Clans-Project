#pragma once

#include "cocos2d.h"
#include "Classes/Core/GameDirector.h"
#include "Classes/Scene/BattleScene.h"
#include "Classes/UI/StoreWindow.h"

// 场景命名空间
USING_NS_CC;

class VillageScene : public Scene
{
public:
    // 场景创建
    static Scene* createScene();

    // 初始化
    virtual bool init() override;

    // 每帧更新
    virtual void update(float dt) override;

    // 按钮回调
    void onAttackButtonClicked(Ref* sender);
    void onShopButtonClicked(Ref* sender);
    void onBuilderButtonClicked(Ref* sender);

    CREATE_FUNC(VillageScene);

private:
    // UI 层与面板
    Layer* _backgroundLayer;
    Layer* _buildingLayer;
    Layer* _uiLayer;
    LayerColor* _grayMask = nullptr;
    Node* _attackPanel = nullptr;
    Node* _marketPanel = nullptr;

    // 与商店相关：StoreWindow 实例（分离出来的部分）
    StoreWindow* _storeWindow = nullptr;

    // 内部回调
    void onMarketButtonClicked(Ref* sender);
    void closeAttackPanel(Ref* sender);

    // 放置模式（当从商店选择建筑）
    void enterPlacementMode(int buildingType);

    // 缩放/拖动节点相关
    Node* _scrollNode;
    Sprite* _backgroundSprite;
    Vec2 _lastTouchPos;
    bool _isDragging;
    float _scaleMin;
    float _scaleMax;
    Size _backgroundSize;
    Size _visibleSize;

    // 功能函数
    void initScrollNode();
    void registerMouseEvents();
    void onMouseScroll(EventMouse* event);
    void onMouseDown(EventMouse* event);
    void onMouseMove(EventMouse* event);
    void onMouseUp(EventMouse* event);
    void clampScrollNodePosition();
    void clampScrollNodeScale(float targetScale);
};