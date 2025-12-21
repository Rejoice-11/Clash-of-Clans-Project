// 必须包含对应的头文件
#include "VillageScene.h"

// 场景创建函数实现
Scene* VillageScene::createScene()
{
    auto scene = Scene::create();
    auto layer = VillageScene::create();
    if (layer) {
        scene->addChild(layer);
    }
    return scene;
}

// 初始化函数实现
bool VillageScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    _visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 初始化缩放拖动节点（核心）
    initScrollNode();
     // 绘制网格
    GridUtils::drawGrid(_scrollNode);
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

// 初始化缩放拖动节点实现
void VillageScene::initScrollNode()
{
    // 创建承载背景的可交互节点
    _scrollNode = Node::create();
    _scrollNode->setPosition(_visibleSize.width / 2, _visibleSize.height / 2);
    this->addChild(_scrollNode, -1);

    // 加载背景精灵
    _backgroundSprite = Sprite::create("2village_background.jpg");
    if (_backgroundSprite) {
        _backgroundSize = _backgroundSprite->getContentSize();
        _backgroundSprite->setPosition(Vec2::ZERO); // 相对于scrollNode居中
        _scrollNode->addChild(_backgroundSprite);

        // 计算最小/最大缩放比例（保证无黑边）
        // 最小缩放：背景刚好覆盖屏幕
        _scaleMin = std::max(_visibleSize.width / _backgroundSize.width,
            _visibleSize.height / _backgroundSize.height);
        // 最大缩放：限制放大倍数（可根据需求调整，比如3倍最小缩放）
        _scaleMax = _scaleMin * 3.0f;

        // 初始缩放为最小缩放（刚好覆盖屏幕）
        _scrollNode->setScale(_scaleMin);
    }

    _isDragging = false;
}

// 注册鼠标事件实现
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

// 鼠标滚轮缩放实现
void VillageScene::onMouseScroll(EventMouse* event)
{
    if (!_backgroundSprite) return;

    if (_attackPanel)
    {
        return;  // 攻击模式下不缩放地图
    }

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

// 鼠标按下实现
void VillageScene::onMouseDown(EventMouse* event)
{
    if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
    {
        if (_isPlacementMode) cancelPlacementMode();  // 右键取消
        return;
    }

    if (event->getMouseButton() != EventMouse::MouseButton::BUTTON_LEFT) return;

    Vec2 mousePos = Vec2(event->getCursorX(), event->getCursorY());
    Vec2 worldPos = this->convertToNodeSpace(mousePos);

    if (_isPlacementMode)
    {
        confirmPlacement(worldPos);  // 左键确认放置
        return;
    }

    _isDragging = true;
    _lastTouchPos = worldPos;
}

// 鼠标移动实现
void VillageScene::onMouseMove(EventMouse* event)
{
    Vec2 mousePos = Vec2(event->getCursorX(), event->getCursorY());
    Vec2 worldPos = this->convertToNodeSpace(mousePos);  // 屏幕→世界坐标

    if (_isPlacementMode ||_attackPanel )
    {
        updateGhostPosition(worldPos);
        return;  // 放置,商店，攻击模式下不拖动地图
    }

    if (!_isDragging || !_backgroundSprite) return;

    Vec2 currentTouchPos = this->convertToNodeSpace(mousePos);
    Vec2 delta = currentTouchPos - _lastTouchPos;
    _scrollNode->setPosition(_scrollNode->getPosition() + delta);
    _lastTouchPos = currentTouchPos;
    clampScrollNodePosition();
}

// 鼠标松开实现
void VillageScene::onMouseUp(EventMouse* event)
{
    if (event->getMouseButton() != EventMouse::MouseButton::BUTTON_LEFT) return;
    _isDragging = false;
}

// 限制scrollNode的位置（无黑边）实现
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

// 限制缩放比例（无黑边）实现
void VillageScene::clampScrollNodeScale(float targetScale)
{
    // 限制在最小/最大缩放之间
    float newScale = clampf(targetScale, _scaleMin, _scaleMax);
    _scrollNode->setScale(newScale);
}

// 攻击按钮点击回调实现
void VillageScene::onAttackButtonClicked(Ref* sender)
{
    if (_attackPanel || (_storeWindow && _storeWindow->isVisible())) return;

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

        fightBtn->setPosition(Vec2(panel->getContentSize().width - 190,
            panel->getContentSize().height - 330));

        auto menu = Menu::create(fightBtn, closeBtn, nullptr);
        menu->setPosition(Vec2::ZERO);
        panel->addChild(menu, 1);
    }
}

// 关闭攻击面板回调实现
void VillageScene::closeAttackPanel(Ref*)
{
    if (_attackPanel) {
        _attackPanel->removeFromParent();
        _attackPanel = nullptr;
    }
    _grayMask->setVisible(false);
}

void VillageScene::onMarketButtonClicked(Ref* sender)
{
    if (_attackPanel || _isPlacementMode) return;  // 放置,攻击中不能开商店

    if (!_storeWindow)
    {
        // 关键！回调带建筑类型
        _storeWindow = StoreWindow::create([this](StoreWindow::BuildingType type) {
            this->enterPlacementMode(type);
            });
        this->addChild(_storeWindow, 30);
    }
    _storeWindow->show();
}

// 获取幽灵建筑精灵名称（根据类型）
void VillageScene::enterPlacementMode(StoreWindow::BuildingType type)
{
    _isPlacementMode = true;
    _pendingBuildingType = type;

    // 创建幽灵建筑（半透明）
    std::string spriteName = getGhostSpriteName(type);
    _ghostBuilding = Sprite::create(spriteName);
    if (_ghostBuilding)
    {
        _ghostBuilding->setOpacity(150);  // 半透明
        _ghostBuilding->setColor(Color3B::GREEN);  // 先绿表示可建（后面加网格判断改红/绿）
        this->addChild(_ghostBuilding, 50);  // 最上层
    }

    // 隐藏商店遮罩（StoreWindow自己hide了，但保险起见）
    _grayMask->setVisible(false);
}

//加取消和确认函数
void VillageScene::cancelPlacementMode()
{
    if (_ghostBuilding)
    {
        _ghostBuilding->removeFromParent();
        _ghostBuilding = nullptr;
    }
    _isPlacementMode = false;
    _pendingBuildingType = StoreWindow::BuildingType::MAX_TYPES;
}

void VillageScene::confirmPlacement(const Vec2& worldPos)
{
    // 这里以后真正创建Building实例 + 扣资源 + 分配工人
    // 先用假建筑占位（和存档那套一样）
    std::string spriteName = getGhostSpriteName(_pendingBuildingType);
    auto realBuilding = Sprite::create(spriteName);
    if (realBuilding)
    {
        realBuilding->setPosition(worldPos);
        _scrollNode->addChild(realBuilding, 5);  // 建筑放在scrollNode里，随地图移动
        // 计数+1（以TownHall为例）
        if (_pendingBuildingType == StoreWindow::BuildingType::TOWN_HALL)
            countofTownHallsInVillage++;
    }

    cancelPlacementMode();  // 放置完自动退出模式
}



// 新增：更新幽灵位置（对齐到scrollNode坐标系）
void VillageScene::updateGhostPosition(const Vec2& mouseWorldPos)
{
    if (!_ghostBuilding) return;

    // 将鼠标世界坐标转换为scrollNode本地坐标
    Vec2 localPos = _scrollNode->convertToNodeSpace(mouseWorldPos);
    // 以后可以加网格对齐：localPos = GridUtils::snapToGrid(localPos);
    Vec2 ghostWorldPos = _scrollNode->convertToWorldSpace(localPos);

    _ghostBuilding->setPosition(ghostWorldPos);
}

// 新增：根据类型拿图
std::string VillageScene::getGhostSpriteName(StoreWindow::BuildingType type)
{
    switch (type) {
    case StoreWindow::BuildingType::TOWN_HALL: return "town_hall_lv1.png";
    case StoreWindow::BuildingType::GOLD_MINE: return "gold_mine_lv1.png";
        // ... 其他同理 ...
    default: return "town_hall_lv1.png";
    }
}

// 帧更新函数实现
void VillageScene::update(float dt)
{
    // 资源生产、建造倒计时等逻辑
}

// 未实现的空函数（保持代码完整性）
void VillageScene::onShopButtonClicked(Ref* sender) {}
void VillageScene::onBuilderButtonClicked(Ref* sender) {}