#pragma once

#include "cocos2d.h"
#include "Classes/Core/GameDirector.h"
#include "Classes/Scene/BattleScene.h"

// 命名空间声明
USING_NS_CC;

class VillageScene : public Scene
{
public:
    // 场景创建函数
    static Scene* createScene();

    // 初始化函数
    virtual bool init() override;

    // 帧更新函数
    virtual void update(float dt) override;

    // 按钮回调声明
    void onAttackButtonClicked(Ref* sender);
    void onShopButtonClicked(Ref* sender);
    void onBuilderButtonClicked(Ref* sender);

    // Cocos2d-x 宏，用于创建实例
    CREATE_FUNC(VillageScene);

private:
    // 原有成员变量
    Layer* _backgroundLayer;
    Layer* _buildingLayer;
    Layer* _uiLayer;
    LayerColor* _grayMask = nullptr;
    Node* _attackPanel = nullptr;
    Node* _marketPanel = nullptr;

    // 内部回调函数声明
    void onMarketButtonClicked(Ref* sender);
    void closeAttackPanel(Ref* sender);

    // 缩放拖动相关变量（新增）
    Node* _scrollNode;          // 承载背景的可缩放/拖动节点
    Sprite* _backgroundSprite;  // 背景精灵
    Vec2 _lastTouchPos;         // 上次触摸位置（用于拖动）
    bool _isDragging;           // 是否正在拖动
    float _scaleMin;            // 最小缩放比例
    float _scaleMax;            // 最大缩放比例
    Size _backgroundSize;       // 背景原始尺寸
    Size _visibleSize;          // 可视区域尺寸

    // 缩放拖动相关函数声明（新增）
    void initScrollNode();                      // 初始化缩放拖动节点
    void registerMouseEvents();                 // 注册鼠标事件
    void onMouseScroll(EventMouse* event);      // 鼠标滚轮缩放
    void onMouseDown(EventMouse* event);        // 鼠标按下
    void onMouseMove(EventMouse* event);        // 鼠标移动
    void onMouseUp(EventMouse* event);          // 鼠标松开
    void clampScrollNodePosition();             // 限制节点位置（无黑边）
    void clampScrollNodeScale(float targetScale); // 限制缩放比例（无黑边）
};