#pragma once

#include "cocos2d.h"
#include "Classes/Core/GameDirector.h"
#include "Classes/Scene/BattleScene.h"
USING_NS_CC;

class VillageScene : public Scene
{
public:
    static Scene* createScene();
    virtual bool init() override;
    virtual void update(float dt) override;

    // 按钮回调
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
    Node* _marketPanel = nullptr;
    void onMarketButtonClicked(Ref* sender);
    void closeAttackPanel(Ref* sender);

    // 新增：缩放拖动相关变量
    Node* _scrollNode;          // 承载背景的可缩放/拖动节点
    Sprite* _backgroundSprite;  // 背景精灵
    Vec2 _lastTouchPos;         // 上次触摸位置（用于拖动）
    bool _isDragging;           // 是否正在拖动
    float _scaleMin;            // 最小缩放比例
    float _scaleMax;            // 最大缩放比例
    Size _backgroundSize;       // 背景原始尺寸
    Size _visibleSize;          // 可视区域尺寸

    // 新增：事件处理函数
    void initScrollNode();                      // 初始化缩放拖动节点
    void registerMouseEvents();                 // 注册鼠标事件
    void onMouseScroll(EventMouse* event);      // 鼠标滚轮缩放
    void onMouseDown(EventMouse* event);        // 鼠标按下
    void onMouseMove(EventMouse* event);        // 鼠标移动
    void onMouseUp(EventMouse* event);          // 鼠标松开
    void clampScrollNodePosition();             // 限制节点位置（无黑边）
    void clampScrollNodeScale(float targetScale); // 限制缩放比例（无黑边）
};
Scene* VillageScene::createScene()
{
    auto scene = Scene::create();
    auto layer = VillageScene::create();
    if (layer) {
        scene->addChild(layer);
    }
    return scene;
}

bool VillageScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    _visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 初始化缩放拖动节点（核心）
    initScrollNode();

    // 2. UI按钮（固定在屏幕角落，不随背景变化）
    // 左下角 攻击按钮
    auto attackBtn = MenuItemImage::create(
        "attack_button.png", "attack_button.png",
        CC_CALLBACK_1(VillageScene::onAttackButtonClicked, this));
    attackBtn->setPosition(Vec2(100 + origin.x, 100 + origin.y));

    // 右下角 商店按钮
    auto marketBtn = MenuItemImage::create(
        "market_button.png", "market_button.png",
        CC_CALLBACK_1(VillageScene::onMarketButtonClicked, this));
    marketBtn->setPosition(Vec2(_visibleSize.width - 100 + origin.x, 100 + origin.y));

    auto menu = Menu::create(attackBtn, marketBtn, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 10); // UI层级高于背景

    // 预先创建遮罩层
    _grayMask = LayerColor::create(Color4B(0, 0, 0, 180), _visibleSize.width, _visibleSize.height);
    _grayMask->setPosition(origin);
    _grayMask->setVisible(false);
    this->addChild(_grayMask, 20);

    // 注册鼠标事件
    registerMouseEvents();

    return true;
}

// 初始化缩放拖动节点
void VillageScene::initScrollNode()
{
    // 创建承载背景的可交互节点
    _scrollNode = Node::create();
    _scrollNode->setPosition(_visibleSize.width / 2, _visibleSize.height / 2);
    this->addChild(_scrollNode, -1);

    // 加载背景精灵
    _backgroundSprite = Sprite::create("village_background.jpg");
    if (_backgroundSprite) {
        _backgroundSize = _backgroundSprite->getContentSize();
        _backgroundSprite->setPosition(Vec2::ZERO); // 相对于scrollNode居中
        _scrollNode->addChild(_backgroundSprite);

        // 计算最小/最大缩放比例（保证无黑边）
        // 最小缩放：背景刚好覆盖屏幕
        _scaleMin = std::max(_visibleSize.width / _backgroundSize.width,
            _visibleSize.height / _backgroundSize.height);
        // 最大缩放：限制放大倍数（可根据需求调整，比如2倍最小缩放）
        _scaleMax = _scaleMin * 3.0f;

        // 初始缩放为最小缩放（刚好覆盖屏幕）
        _scrollNode->setScale(_scaleMin);
    }

    _isDragging = false;
}

// 注册鼠标事件
void VillageScene::registerMouseEvents()
{
    // 鼠标滚轮事件
    auto mouseScrollListener = EventListenerMouse::create();
    mouseScrollListener->onMouseScroll = CC_CALLBACK_1(VillageScene::onMouseScroll, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseScrollListener, this);

    // 鼠标按下事件
    auto mouseDownListener = EventListenerMouse::create();
    mouseDownListener->onMouseDown = CC_CALLBACK_1(VillageScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseDownListener, this);

    // 鼠标移动事件
    auto mouseMoveListener = EventListenerMouse::create();
    mouseMoveListener->onMouseMove = CC_CALLBACK_1(VillageScene::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseMoveListener, this);

    // 鼠标松开事件
    auto mouseUpListener = EventListenerMouse::create();
    mouseUpListener->onMouseUp = CC_CALLBACK_1(VillageScene::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseUpListener, this);
}

// 鼠标滚轮缩放（上滚缩小，下滚放大）
void VillageScene::onMouseScroll(EventMouse* event)
{
    if (!_backgroundSprite) return;

    // 获取滚轮偏移（y轴：上滚为正，下滚为负）
    float scrollY = event->getScrollY();
    if (scrollY == 0) return;

    // 计算目标缩放比例（上滚缩小，下滚放大）
    float currentScale = _scrollNode->getScale();
    float scaleStep = 0.1f; // 每次滚轮的缩放步长（可调整）
    float targetScale = scrollY > 0 ? currentScale - scaleStep : currentScale + scaleStep;

    // 限制缩放范围（无黑边）
    clampScrollNodeScale(targetScale);

    // 获取鼠标在世界坐标系的位置
    Vec2 mouseWorldPos = _scrollNode->getParent()->convertToNodeSpace(Vec2(event->getCursorX(), event->getCursorY()));
    // 计算缩放中心（鼠标位置相对于scrollNode的偏移）
    Vec2 offset = mouseWorldPos - _scrollNode->getPosition();
    offset = offset / currentScale; // 转换为缩放前的偏移

    // 缩放后调整位置，保证鼠标指向的位置不变（放大镜效果）
    _scrollNode->setScale(_scrollNode->getScale());
    _scrollNode->setPosition(mouseWorldPos - offset * _scrollNode->getScale());

    // 限制位置，防止黑边
    clampScrollNodePosition();
}

// 鼠标按下（左键开始拖动）
void VillageScene::onMouseDown(EventMouse* event)
{
    // 只响应左键
    if (event->getMouseButton() != EventMouse::MouseButton::BUTTON_LEFT) return;

    _isDragging = true;
    // 记录按下时的鼠标位置（世界坐标系）
    _lastTouchPos = _scrollNode->getParent()->convertToNodeSpace(Vec2(event->getCursorX(), event->getCursorY()));
}

// 鼠标移动（拖动背景）
void VillageScene::onMouseMove(EventMouse* event)
{
    if (!_isDragging || !_backgroundSprite) return;

    // 获取当前鼠标位置
    Vec2 currentTouchPos = _scrollNode->getParent()->convertToNodeSpace(Vec2(event->getCursorX(), event->getCursorY()));
    // 计算偏移量
    Vec2 delta = currentTouchPos - _lastTouchPos;

    // 移动scrollNode
    _scrollNode->setPosition(_scrollNode->getPosition() + delta);
    // 更新上次触摸位置
    _lastTouchPos = currentTouchPos;

    // 限制位置，防止黑边
    clampScrollNodePosition();
}

// 鼠标松开（结束拖动）
void VillageScene::onMouseUp(EventMouse* event)
{
    if (event->getMouseButton() != EventMouse::MouseButton::BUTTON_LEFT) return;
    _isDragging = false;
}

// 限制scrollNode的位置，保证无黑边
void VillageScene::clampScrollNodePosition()
{
    if (!_backgroundSprite) return;

    float currentScale = _scrollNode->getScale();
    // 计算scrollNode的有效移动范围
    float maxX = _visibleSize.width / 2 + (_backgroundSize.width * currentScale - _visibleSize.width) / 2;
    float minX = _visibleSize.width / 2 - (_backgroundSize.width * currentScale - _visibleSize.width) / 2;
    float maxY = _visibleSize.height / 2 + (_backgroundSize.height * currentScale - _visibleSize.height) / 2;
    float minY = _visibleSize.height / 2 - (_backgroundSize.height * currentScale - _visibleSize.height) / 2;

    // 限制x、y轴位置
    Vec2 newPos = _scrollNode->getPosition();
    newPos.x = clampf(newPos.x, minX, maxX);
    newPos.y = clampf(newPos.y, minY, maxY);
    _scrollNode->setPosition(newPos);
}

// 限制缩放比例，保证无黑边
void VillageScene::clampScrollNodeScale(float targetScale)
{
    // 限制在最小/最大缩放之间
    float newScale = clampf(targetScale, _scaleMin, _scaleMax);
    _scrollNode->setScale(newScale);
}

// 攻击按钮点击 → 弹出左侧攻击面板
void VillageScene::onAttackButtonClicked(Ref* sender)
{
    if (_attackPanel || _marketPanel) return;

    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _grayMask->setVisible(true);

    auto panel = Sprite::create("attack_out_when_point_the_attack_button_in_village_scene.png");
    if (panel) {
        panel->setAnchorPoint(Vec2(0, 0.5f));
        panel->setPosition(Vec2(origin.x, _visibleSize.height / 2 + origin.y));
        this->addChild(panel, 30);
        _attackPanel = panel;

        auto fightBtn = MenuItemImage::create(
            "attack_button_to_reverse_to_battle_scene.png",
            "attack_button_to_reverse_to_battle_scene.png",
            [this](Ref*) {
                this->closeAttackPanel(nullptr);
                GameDirector::getInstance()->replaceScene(BattleScene::createScene());
            });

        auto closeBtn = MenuItemImage::create(
            "out_of_now.png", "out_of_now.png",
            CC_CALLBACK_1(VillageScene::closeAttackPanel, this));

        closeBtn->setPosition(Vec2(panel->getContentSize().width - 40,
            panel->getContentSize().height - 40));

        fightBtn->setPosition(panel->getContentSize() / 2);

        auto menu = Menu::create(fightBtn, closeBtn, nullptr);
        menu->setPosition(Vec2::ZERO);
        panel->addChild(menu, 1);
    }
}

// 关闭攻击面板
void VillageScene::closeAttackPanel(Ref*)
{
    if (_attackPanel) {
        _attackPanel->removeFromParent();
        _attackPanel = nullptr;
    }
    _grayMask->setVisible(false);
}

// 商店按钮点击 → 全屏商店
void VillageScene::onMarketButtonClicked(Ref* sender)
{
    if (_attackPanel || _marketPanel) return;

    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _grayMask->setVisible(true);

    auto market = Sprite::create("basic_market_bar.png");
    if (market) {
        auto centerPos = Vec2(_visibleSize.width / 2 + origin.x, _visibleSize.height / 2 + origin.y);
        market->setPosition(centerPos);
        this->addChild(market, 30);
        _marketPanel = market;

        auto closeBtn = MenuItemImage::create(
            "out_of_now.png", "out_of_now.png",
            [this](Ref*) {
                if (_marketPanel) {
                    _marketPanel->removeFromParent();
                    _marketPanel = nullptr;
                }
                _grayMask->setVisible(false);
            });

        closeBtn->setPosition(Vec2(market->getContentSize().width - 60,
            market->getContentSize().height - 60));

        auto menu = Menu::create(closeBtn, nullptr);
        menu->setPosition(Vec2::ZERO);
        market->addChild(menu, 1);
    }
}

void VillageScene::update(float dt)
{
    // 资源生产、建造倒计时等逻辑
}

// 以下为未实现的空函数（保持代码完整性）
void VillageScene::onShopButtonClicked(Ref* sender) {}
void VillageScene::onBuilderButtonClicked(Ref* sender) {}