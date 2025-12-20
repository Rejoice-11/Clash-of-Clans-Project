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
};