// 必须包含对应的头文件//unfinished
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
    // 添加设置按钮（左上角）
    addSettingButton();
    //创建设置层（但不显示）
    _settingLayer = SettingLayer::create();
    if (_settingLayer) {
        // 设置回调
        _settingLayer->setExitCallback([this]() {
            this->exitGame();
            });
        _settingLayer->setCloseCallback([this]() {
            // 设置层关闭后的操作
            CCLOG("Setting layer closed");
            });

        // 加载音频设置并播放背景音乐
        _settingLayer->loadAudioSettings();

        // 添加到场景，但初始隐藏
        this->addChild(_settingLayer, 1);
    }
    // 初始化缩放拖动节点（核心）
    initScrollNode();
    // 绘制网格
    GridUtils::drawGrid(_scrollNode);
    //UI按钮（固定在屏幕角落，不随背景变化）
    auto hud = HUDLayer::create();
    this->addChild(hud, 5); // 高层级，确保在最上层
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
    // 预加载按钮点击音效
    SimpleAudioEngine::getInstance()->preloadEffect("audio/button_click.mp3");
    return true;
}
// 进入场景时调用
void VillageScene::onEnter()
{
    Scene::onEnter();
    // 播放背景音乐
    SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/background.mp3", true);
}

// 退出场景时调用
void VillageScene::onExit()
{
    Scene::onExit();
    // 保存音频设置
    if (_settingLayer) {
        _settingLayer->saveAudioSettings();
    }
}
// 添加设置按钮到左上角
void VillageScene::addSettingButton()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // 尝试加载设置按钮图片
    _settingButton = MenuItemImage::create(
        "setting_normal.png",          // 正常状态图片
        "setting_normal.png",        // 选中状态图片
        CC_CALLBACK_1(VillageScene::onSettingButtonClicked, this));
    _settingButton->setScale(0.2f);
    // 如果图片不存在，创建文本按钮
    if (!_settingButton) {
        // 创建齿轮图标作为设置按钮
        auto drawNode = DrawNode::create();
        // 画齿轮形状
        drawNode->drawCircle(Vec2(0, 0), 20, 0, 20, false, 1, 1, Color4F::WHITE);
        for (int i = 0; i < 8; i++) {
            float angle = i * (M_PI / 4);
            Vec2 start(20 * cos(angle), 20 * sin(angle));
            Vec2 end(30 * cos(angle), 30 * sin(angle));
            drawNode->drawLine(start, end, Color4F::WHITE);
        }

        _settingButton = MenuItemSprite::create(drawNode, drawNode,
            CC_CALLBACK_1(VillageScene::onSettingButtonClicked, this));

        // 设置按钮大小
        _settingButton->setScale(0.1f);
    }

    auto menu = Menu::create(_settingButton, nullptr);
    menu->setPosition(Vec2(40, visibleSize.height - 40)); // 左上角位置
    this->addChild(menu, 100); // 确保在最上层
}
// 设置按钮点击回调
void VillageScene::onSettingButtonClicked(Ref* sender)
{
    if (_settingLayer) {
        // 播放按钮点击音效
        _settingLayer->playButtonClickSound();
        // 显示设置层
        _settingLayer->show();
    }
}
// 退出游戏
void VillageScene::exitGame()
{
    // 播放退出音效
    SimpleAudioEngine::getInstance()->playEffect("audio/button_click.mp3");

    // 延迟退出，确保音效播放
    this->runAction(Sequence::create(
        DelayTime::create(0.3f),
        CallFunc::create([=]() {
            // 停止所有音频
            SimpleAudioEngine::getInstance()->stopAllEffects();
            SimpleAudioEngine::getInstance()->stopBackgroundMusic();

            // 退出游戏
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
#else
            Director::getInstance()->end();
#endif
            }),
        nullptr
    ));
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
	//之前通过这个写了很多功能，但因水平不够，我们无法维护这些代码了，但舍不得删，留作纪念/(ㄒoㄒ)/~~
    /*if (!_backgroundSprite) return;

    if (_attackPanel || _isPlacementMode)
    {
        return;  // 攻击模式和放置模式下不缩放地图
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
    clampScrollNodePosition();    */
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
    if (_isPlacementMode )
    {
        updateGhostPosition(worldPos);
        return; 
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
    if (_settingLayer) {
        _settingLayer->playButtonClickSound(); // 添加音效
    }
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
                GameDirector::getInstance()->pushScene(BattleScene::createScene());
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
    if (_settingLayer) {
        _settingLayer->playButtonClickSound(); // 添加音效
    }
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

    // 强制缩放到初始大小（原来的样子）
    _scrollNode->setScale(_scaleMin);
    // 重置位置到屏幕中心（避免缩放后位置偏移）
    _scrollNode->setPosition(_visibleSize.width / 2, _visibleSize.height / 2);
    // 确保位置在有效范围内
    clampScrollNodePosition();

    // 获取幽灵建筑图片名称
    std::string spriteName = getGhostSpriteName(type);
    if (spriteName.empty()) 
    {
        CCLOG("Error: 找不到建筑对应的精灵图片");
        return;
    }

    // 创建幽灵建筑（带透明度的半透明效果）
    _ghostBuilding = Sprite::create(spriteName);
    if (!_ghostBuilding) 
    {
        CCLOG("Error: 无法创建幽灵建筑，图片路径可能错误: %s", spriteName.c_str());
        return;
    }

    // 设置幽灵建筑属性
    _ghostBuilding->setOpacity(150);  // 半透明
    _ghostBuilding->setColor(Color3B::GREEN);  // 默认绿色（可放置状态）
    this->addChild(_ghostBuilding, 50);  // 确保层级在最上层

    // 隐藏商店窗口和灰色遮罩
    if (_storeWindow) 
    {
        _storeWindow->hide();
    }
    _grayMask->setVisible(false);

    // 初始化位置（防止首次创建时位置错误）
    Vec2 initPos = Director::getInstance()->getVisibleSize() / 2;
    updateGhostPosition(initPos);
}

//加取消和确认函数
void VillageScene::cancelPlacementMode() 
{
    if (_ghostBuilding) 
    {
        _ghostBuilding->removeFromParent();
        _ghostBuilding = nullptr;
    }

    if (_isMovingBuilding) 
    {
        // 如果是取消移动，要把原精灵加回来！
        if (_movingSprite && _movingBuilding) 
        {
            // 重新设置位置（用原 gridPosition）
            Vec2 gridPos = _movingBuilding->getGridPosition();
            Vec2 finalPos = GridUtils::gridToWorld(gridPos);
            _movingSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
            _movingSprite->setPosition(finalPos);
            _scrollNode->addChild(_movingSprite, 5);

            // 重新绑定点击（可选）
            // （如果没删监听器，可能不需要）
        }
        _movingBuilding = nullptr;
        _movingSprite = nullptr;
        _isMovingBuilding = false;
    }

    _isPlacementMode = false;
    _pendingBuildingType = StoreWindow::BuildingType::MAX_TYPES;
}

void VillageScene::confirmPlacement(const Vec2& worldPos) 
{
    if (!_ghostBuilding || !_isPlacementMode) 
        return;

    Vec2 gridPos = GridUtils::worldToGrid(worldPos);
    gridPos.x = floorf(gridPos.x);
    gridPos.y = floorf(gridPos.y);

    if (!GridUtils::isBuildingInDiamond(gridPos))
    {
        CCLOG("无法放置：超出菱形范围");
        return;
    }

    if (_isMovingBuilding && _movingBuilding) 
    {
        // === 移动模式 ===
        // 1. 更新建筑的 gridPosition
        _movingBuilding->setGridPosition(gridPos);

        // 2. 重新创建真实精灵（用原图）
        std::string spriteName = getGhostSpriteName(_pendingBuildingType);
        auto newSprite = Sprite::create(spriteName);
        if (!newSprite) 
        {
            CCLOG("Failed to recreate sprite after move");
            return;

        }

        // 3. 设置位置（注意坐标转换）
        Vec2 Pos = GridUtils::gridToWorld(gridPos);
        Vec2 finalPos = Vec2(Pos.x - 640, Pos.y - 360);
        newSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        newSprite->setPosition(finalPos);

        // 4. 绑定建筑对象（关键！）
        newSprite->setUserObject(_movingBuilding); // ← 指向同一个建筑！

        // 5. 添加到场景
        _scrollNode->addChild(newSprite, 5);

        // 6. 重新绑定点击事件（可选，如果之前有）
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [this, newSprite](Touch* touch, Event* event) 
        {
            Vec2 touchPos = touch->getLocation();
            Vec2 nodePos = newSprite->getParent()->convertToNodeSpace(touchPos);
            Rect bounds(newSprite->getPosition() - newSprite->getContentSize() / 2,
                        newSprite->getContentSize());

            if (bounds.containsPoint(nodePos)) 
            {
                this->onBuildingClicked(newSprite);
                return true;
            }
            return false;
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, newSprite);

        // 7. 清理移动状态
        _movingBuilding = nullptr;
        _movingSprite = nullptr;
        _isMovingBuilding = false;
    }

    else 
    {
        cocos2d::Sprite* realSprite = nullptr;
        // 创建建筑对象

        switch (_pendingBuildingType)
        {
        case StoreWindow::BuildingType::TOWN_HALL:
        {
            auto th = std::make_unique<TownHall>(TownHallBuildingData);
            th->setGridPosition(gridPos);
            _townHalls.push_back(std::move(th));

            // 创建精灵
            std::string spriteName = getGhostSpriteName(_pendingBuildingType);
            realSprite = Sprite::create(spriteName);

            // 直接绑定建筑指针！
            realSprite->setUserObject(_townHalls.back().get()); // .get() 返回裸指针
            break;
        }

        case StoreWindow::BuildingType::GOLD_MINE:
        {
            auto mine = std::make_unique<ResourceBuilding>(GoldMineBuildingData, -1, ResourceBuilding::ResourceType::GOLD);
            mine->setGridPosition(gridPos);
            _goldMines.push_back(std::move(mine));

            // 创建精灵
            std::string spriteName = getGhostSpriteName(_pendingBuildingType);
            realSprite = Sprite::create(spriteName);

            // 直接绑定建筑指针！
            realSprite->setUserObject(_goldMines.back().get()); // .get() 返回裸指针
            break;
        }

        case StoreWindow::BuildingType::ELIXIR_COLLECTOR:
        {
            auto collector = std::make_unique<ResourceBuilding>(ElixirCollectorBuildingData, -1, ResourceBuilding::ResourceType::ELIXIR);
            collector->setGridPosition(gridPos);
            _elixirCollectors.push_back(std::move(collector));

            // 创建精灵
            std::string spriteName = getGhostSpriteName(_pendingBuildingType);
            realSprite = Sprite::create(spriteName);

            // 直接绑定建筑指针！
            realSprite->setUserObject(_elixirCollectors.back().get()); // .get() 返回裸指针
            break;
        }

        case StoreWindow::BuildingType::GOLD_STORAGE:
        {
            auto storage = std::make_unique<StorageBuilding>(GoldStorageBuildingData, -1, StorageBuilding::StorageType::GOLD_STORAGE);
            storage->setGridPosition(gridPos);
            _goldStorages.push_back(std::move(storage));

            // 创建精灵
            std::string spriteName = getGhostSpriteName(_pendingBuildingType);
            realSprite = Sprite::create(spriteName);

            // 直接绑定建筑指针！
            realSprite->setUserObject(_goldStorages.back().get()); // .get() 返回裸指针
            break;
        }

        case StoreWindow::BuildingType::ARCHER_TOWER:
        {
            auto tower = std::make_unique<DefenseBuilding>(
                ArcherTowerBuildingData,
                -1,
                DefenseBuilding::DefenseType::ARCHER_TOWER
            );
            tower->setGridPosition(gridPos);
            _archerTowers.push_back(std::move(tower));

            // 创建精灵
            std::string spriteName = getGhostSpriteName(_pendingBuildingType);
            realSprite = Sprite::create(spriteName);

            // 直接绑定建筑指针！
            realSprite->setUserObject(_archerTowers.back().get()); // .get() 返回裸指针
            break;
        }

        case StoreWindow::BuildingType::ELIXIR_STORAGE:
        {
            auto storage = std::make_unique<StorageBuilding>(ElixirStorageBuildingData, -1, StorageBuilding::StorageType::ELIXIR_STORAGE);
            storage->setGridPosition(gridPos);
            _elixirStorages.push_back(std::move(storage));
            // 放置后重新计算总容量
            //unfinishd
            // 创建精灵
            std::string spriteName = getGhostSpriteName(_pendingBuildingType);
            realSprite = Sprite::create(spriteName);

            // 直接绑定建筑指针！
            realSprite->setUserObject(_elixirStorages.back().get()); // .get() 返回裸指针
            break;
        }
        /*unfinished
        case StoreWindow::BuildingType::MILITARY_CAMP:
        {
            auto camp = std::make_unique<MilitaryCamp>(MilitaryCampBuildingData);
            camp->setGridPosition(gridPos);
            _militaryCamps.push_back(std::move(camp));
            // 创建精灵
            std::string spriteName = getGhostSpriteName(_pendingBuildingType);
            realSprite = Sprite::create(spriteName);

            // 直接绑定建筑指针！
            realSprite->setUserObject(_militaryCamps.back().get()); // .get() 返回裸指针
            break;
        }
        */

        case StoreWindow::BuildingType::CANNON:
        {
            auto cannon = std::make_unique<DefenseBuilding>(CanonBuildingData, -1, DefenseBuilding::DefenseType::CANON);
            cannon->setGridPosition(gridPos);
            _cannons.push_back(std::move(cannon));

            // 创建精灵
            std::string spriteName = getGhostSpriteName(_pendingBuildingType);
            realSprite = Sprite::create(spriteName);

            // 直接绑定建筑指针！
            realSprite->setUserObject(_cannons.back().get()); // .get() 返回裸指针
            break;
        }
        /*unfinished
        case StoreWindow::BuildingType::WORKER_HOME:
        {
            auto home = std::make_unique<WorkerHome>(WorkerHomeBuildingData);
            home->setGridPosition(gridPos);

            _workerHomes.push_back(std::move(home));
            // 创建精灵
            std::string spriteName = getGhostSpriteName(_pendingBuildingType);
            realSprite = Sprite::create(spriteName);

            // 直接绑定建筑指针！
            realSprite->setUserObject(_workerHomes.back().get()); // .get() 返回裸指针
            break;
        }
        */
        }


        Vec2 finalPos = GridUtils::gridToWorld(gridPos);
        realSprite->setAnchorPoint(Vec2(0.5, 0.5));
        finalPos = Vec2(finalPos.x - 640, finalPos.y - 360);
        realSprite->setPosition(finalPos);
        _scrollNode->addChild(realSprite, 5);

        // 绑定点击事件
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [this, realSprite](Touch* touch, Event* event)
            {
                Vec2 touchPos = touch->getLocation();
                Vec2 nodePos = realSprite->getParent()->convertToNodeSpace(touchPos);
                Rect bounds(realSprite->getPosition() - realSprite->getContentSize() / 2,
                    realSprite->getContentSize());
                if (bounds.containsPoint(nodePos))
                {
                    this->onBuildingClicked(realSprite);
                    return true;
                }
                return false;
            };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, realSprite);
    }

    cancelPlacementMode();
}

void VillageScene::onBuildingClicked(Sprite* sprite)
{
    if (_isInAttackPanel || (_storeWindow && _storeWindow->isVisible()) || _isPlacementMode) 
    {
        return;
    }

    _selectedBuildingSprite = sprite;
    _selectedBuilding = findBuildingBySprite(sprite); // 需要实现
    _selectedBuildingType = getBuildingTypeFromSprite(sprite); // 需要实现

    showBuildingActionPanel();
}

Building* VillageScene::findBuildingBySprite(Sprite* sprite) 
{
    return dynamic_cast<Building*>(sprite->getUserObject());
}

StoreWindow::BuildingType VillageScene::getBuildingTypeFromSprite(Sprite* sprite) 
{
    auto building = findBuildingBySprite(sprite);
    if (!building) 
        return StoreWindow::BuildingType::MAX_TYPES;

	int typeID = building->getBuildingType();

    switch (static_cast<BuildingType>(typeID))
    {
    case BuildingType::TOWN_HALL:
        return StoreWindow::BuildingType::TOWN_HALL;
    case BuildingType::GOLD_MINE:
        return StoreWindow::BuildingType::GOLD_MINE;
    case BuildingType::ELIXIR_COLLECTOR:
        return StoreWindow::BuildingType::ELIXIR_COLLECTOR;
    case BuildingType::GOLD_STORAGE:
        return StoreWindow::BuildingType::GOLD_STORAGE;
    case BuildingType::ELIXIR_STORAGE:
        return StoreWindow::BuildingType::ELIXIR_STORAGE;
    case BuildingType::MILITARY_CAMP:
        return StoreWindow::BuildingType::MILITARY_CAMP;
    case BuildingType::ARCHER_TOWER:
        return StoreWindow::BuildingType::ARCHER_TOWER;
    case BuildingType::CANNON:
        return StoreWindow::BuildingType::CANNON;
    case BuildingType::WORKER_HOME:
        return StoreWindow::BuildingType::WORKER_HOME;
    default:
        return StoreWindow::BuildingType::MAX_TYPES;
    }
}

void VillageScene::showBuildingActionPanel() 
{
    if (_buildingActionPanel) {
        _buildingActionPanel->removeFromParent();
    }

    _grayMask->setVisible(true);
    _isInBuildingActionMode = true;

    auto panel = Node::create();
    panel->setContentSize(Size(300, 80));
    panel->setPosition(Vec2(_visibleSize.width / 2, 40)); // 底部居中

    auto infoBtn = MenuItemImage::create(
        "information_and_upgrade_of_this_building.png",
        "information_and_upgrade_of_this_building.png",
        CC_CALLBACK_1(VillageScene::onInfoButtonClicked, this)
    );
    auto moveBtn = MenuItemImage::create(
        "move_this_building.png",
        "move_this_building.png",
        CC_CALLBACK_1(VillageScene::onMoveButtonClicked, this)
    );
    auto closeBtn = MenuItemImage::create(
        "out_of_now.png",
        "out_of_now.png",
        [this](Ref*) { hideBuildingActionPanel(); }
    );

    infoBtn->setPosition(Vec2(-100, 0));
    moveBtn->setPosition(Vec2(0, 0));
    closeBtn->setPosition(Vec2(100, 0));

    auto menu = Menu::create(infoBtn, moveBtn, closeBtn, nullptr);
    menu->setPosition(Vec2::ZERO);
    panel->addChild(menu);
    this->addChild(panel, 100);
    _buildingActionPanel = panel;
}

void VillageScene::hideBuildingActionPanel() 
{
    if (_buildingActionPanel) 
    {
        _buildingActionPanel->removeFromParent();
        _buildingActionPanel = nullptr;
    }

    _grayMask->setVisible(false);
    _isInBuildingActionMode = false;
    _selectedBuildingSprite = nullptr;
    _selectedBuilding = nullptr;
}

// 实现移动建筑逻辑
void VillageScene::onMoveButtonClicked(Ref* sender) 
{
    if (!_selectedBuilding || !_selectedBuildingSprite) 
    {
        hideBuildingActionPanel();
        return;
    }
    if (_settingLayer) {
        _settingLayer->playButtonClickSound(); // 添加音效
    }
    // 记录要移动的建筑和精灵
    _movingBuilding = _selectedBuilding;
    _movingSprite = _selectedBuildingSprite;

    // 隐藏面板、取消选中
    hideBuildingActionPanel();

    // 进入“移动”放置模式
    _isMovingBuilding = true;
    _isPlacementMode = true;
    _pendingBuildingType = _selectedBuildingType;

    // 移除原精灵（等放置后再加回来）
    _movingSprite->removeFromParent();
    _selectedBuildingSprite = nullptr;
    _selectedBuilding = nullptr;

    // 创建幽灵精灵（半透明）
    std::string spriteName = getGhostSpriteName(_pendingBuildingType);
    _ghostBuilding = Sprite::create(spriteName);
    if (!_ghostBuilding) 
    {
        CCLOG("Error: Failed to create ghost for moving building");
        cancelPlacementMode();
        return;
    }

    _ghostBuilding->setOpacity(150);
    _ghostBuilding->setColor(Color3B::GREEN);
    this->addChild(_ghostBuilding, 50);

    // 初始化幽灵位置（屏幕中心）
    Vec2 initPos = Director::getInstance()->getVisibleSize() / 2;
    updateGhostPosition(initPos);


}



// 新增：更新幽灵位置
void VillageScene::updateGhostPosition(const Vec2& mouseWorldPos)
{
    // 2. 网格吸附处理（确保3x3建筑中心对齐网格）
    Vec2 gridPos = GridUtils::worldToGrid(mouseWorldPos);
    gridPos.x = floorf(gridPos.x);  // 取整到最近格子
    gridPos.y = floorf(gridPos.y);
    // 4. 检查是否在菱形范围内
    bool isInDiamond = GridUtils::isBuildingInDiamond(gridPos);
    _ghostBuilding->setColor(isInDiamond ? Color3B::GREEN : Color3B::RED);
    // 6. 同步缩放（与背景保持一致）
    _ghostBuilding->setScale(_scrollNode->getScale());
    // 5. 转换回世界坐标并设置位置
    Vec2 finalPos = GridUtils::gridToWorld(gridPos);
    _ghostBuilding->setAnchorPoint(Vec2(0.5, 0.5));
    _ghostBuilding->setPosition(finalPos);
}

// 新增：根据类型拿图
std::string VillageScene::getGhostSpriteName(StoreWindow::BuildingType type)
{
    switch (type)
    {
    case StoreWindow::BuildingType::TOWN_HALL: return "town_hall_lv1.png";
    case StoreWindow::BuildingType::GOLD_MINE: return "gold_mine_lv1.png";
    case StoreWindow::BuildingType::ELIXIR_COLLECTOR: return "elixir_collector_lv1.png";
    case StoreWindow::BuildingType::GOLD_STORAGE: return "gold_storage_lv1.png";
    case StoreWindow::BuildingType::ELIXIR_STORAGE: return "elixir_storage_lv1.png";
    case StoreWindow::BuildingType::MILITARY_CAMP: return "military_camp_lv1.png";
    case StoreWindow::BuildingType::ARCHER_TOWER: return "archer_tower_lv1.png";
    case StoreWindow::BuildingType::CANNON: return "canon_lv1.png";
    case StoreWindow::BuildingType::WORKER_HOME: return "worker_home.png";

    }
}

void VillageScene::recalculateMaxStorage()
{
    int totalGold = 0, totalElixir = 0;

    for (const auto& storage : _goldStorages)
    {
        totalGold += storage->getCapacity();
	}

    for (const auto& storage : _elixirStorages)
    {
        totalElixir += storage->getCapacity();
	}

    ResourceManager::getInstance()->updateMaxGoldStorage(totalGold);
    ResourceManager::getInstance()->updateMaxElixirStorage(totalElixir);
}

// VillageScene.cpp
void VillageScene::onInfoButtonClicked(Ref* sender)
{
    if (_settingLayer) 
    {
        _settingLayer->playButtonClickSound();
    }

    // 创建面板
    auto panel = BuildingPanel::create(
        static_cast<Building*>(_selectedBuilding),
        [this]() { this->onBuildingPanelClosed(); }
    );

    hideBuildingActionPanel();
    this->addChild(panel, 100);
}

void VillageScene::onBuildingPanelClosed() 
{
    // 刷新选中建筑的精灵（因为可能升级了）
    if (_selectedBuildingSprite && _selectedBuilding)
    {
        std::string newSpriteName = getGhostSpriteName(_selectedBuildingType);
        newSpriteName = newSpriteName.substr(0, newSpriteName.find("_lv")) +
            "_lv" + std::to_string(_selectedBuilding->getCurrentLevel()) + ".png";

        auto newSprite = Sprite::create(newSpriteName);
        if (newSprite) 
        {
            // 复制位置/锚点
            newSprite->setPosition(_selectedBuildingSprite->getPosition());
            newSprite->setAnchorPoint(_selectedBuildingSprite->getAnchorPoint());
            newSprite->setUserObject(_selectedBuilding);

            // 替换精灵
            _scrollNode->addChild(newSprite, 5);
            _selectedBuildingSprite->removeFromParent();
            _selectedBuildingSprite = newSprite;
        }
    }
}

// 当玩家升级 Storage 时调用（比如在 StoreWindow 放置后）
void VillageScene::onStorageUpgraded(StorageBuilding* storage)
{
    recalculateMaxStorage();
}

// 帧更新函数实现
void VillageScene::update(float dt)
{
    // 资源生产、建造倒计时等逻辑
}

// 未实现的空函数（保持代码完整性）
void VillageScene::onShopButtonClicked(Ref* sender) {}
void VillageScene::onBuilderButtonClicked(Ref* sender) {}