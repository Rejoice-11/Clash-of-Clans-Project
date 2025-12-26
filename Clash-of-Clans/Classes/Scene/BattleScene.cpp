// 必须包含对应的头文件
#include "BattleScene.h"
#include "Classes/System/ArmyManager.h"
#include "Classes/Entity/Unit/UnitFactory.h"

// 更新箭头位置实现
void BattleScene::updateArrowPosition(cocos2d::MenuItemImage* targetBtn) {
    if (!targetBtn || !_arrowIndicator) 
        return;

    // 1. 获取按钮的世界坐标（自动适配Menu的坐标偏移和缩放）
    Vec2 btnWorldPos = targetBtn->convertToWorldSpace(Vec2(targetBtn->getContentSize().width / 2, targetBtn->getContentSize().height / 2));
    // 2. 转换为当前Layer的本地坐标
    Vec2 btnLocalPos = this->convertToNodeSpace(btnWorldPos);
    // 3. 设置箭头位置（按钮正上方，可调整y偏移量）
    _arrowIndicator->setPosition(Vec2(btnLocalPos.x, 120));
    // 说明：
    // - targetBtn->getContentSize().height/2 * targetBtn->getScale()：按钮缩放后的半高
    // - +10：箭头与按钮的间距，可根据需要调整
}
// 士兵生成实现（预留，后续完善具体逻辑）
void BattleScene::spawnSoldierAtPosition(const Vec2& position)
{
    if (_selectedType == UnitType::NONE) {
        CCLOG("Spawn Failed: No soldier selected!");
        return;
    }

    // 1. 范围校验 (150线以上且在网格内)
    if (position.y < GameConfig::DEPLOY_BOUNDARY_Y) {
        CCLOG("Spawn Failed: Below deployment line (y < %f)", GameConfig::DEPLOY_BOUNDARY_Y);
        return;
    }

    if (!GridUtils::isPointInDiamondBattle(position)) {
        CCLOG("Spawn Failed: Not inside grid diamond!");
        return;
    }

    // 2. 尝试从管理器扣除兵力
    if (ArmyManager::getInstance()->tryDeploy(_selectedType)) {

        // 将玩家点击的坐标转换成最近的网格中心点坐标
        Vec2 snappedPos = GridUtils::snapToGrid(position);


        // 3. 通过工厂生产士兵
        auto newUnit = UnitFactory::createUnit(_selectedType);
        if (newUnit) {
            newUnit->setPosition(snappedPos);
            auto sprite = newUnit->createSprite();
            this->addChild(sprite, GameConfig::Z_UNIT);

            // 可以在这里加上一个小的烟雾特效或声音
        }

        // 4. 刷新 UI
        refreshUI();
    }
}

void BattleScene::refreshUI() {
	for (auto const& [type, btn] : _unitButtons) {//我这里用了C++17的结构化绑定，现代特性
        int count = ArmyManager::getInstance()->getRemainingCount(type);

        // 更新数字显示
        if (_unitLabels.count(type)) {
            _unitLabels[type]->setString(std::to_string(count));
        }

        // 核心逻辑：数量为 0 则变灰并禁用
        if (count <= 0) {
            btn->setEnabled(false);
            btn->setColor(Color3B(100, 100, 100)); // 变灰

            // 如果当前选中的正好是这个用完的兵，取消选中状态
            if (_selectedType == type) {
                _selectedType = UnitType::NONE;
                _selectedSoldierBtn = nullptr;
                _arrowIndicator->setVisible(false);
            }
        }
    }
}



// 鼠标事件注册实现
void BattleScene::registerMouseEvents2()
{
    // 鼠标滚轮事件
    auto mouseScrollListener = EventListenerMouse::create();
    mouseScrollListener->onMouseScroll = CC_CALLBACK_1(BattleScene::onMouseScroll2, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseScrollListener, this);

    // 鼠标按下事件
    auto mouseDownListener = EventListenerMouse::create();
    mouseDownListener->onMouseDown = CC_CALLBACK_1(BattleScene::onMouseDown2, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseDownListener, this);

    // 鼠标移动事件
    auto mouseMoveListener = EventListenerMouse::create();
    mouseMoveListener->onMouseMove = CC_CALLBACK_1(BattleScene::onMouseMove2, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseMoveListener, this);

    // 鼠标松开事件
    auto mouseUpListener = EventListenerMouse::create();
    mouseUpListener->onMouseUp = CC_CALLBACK_1(BattleScene::onMouseUp2, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseUpListener, this);
}
// 鼠标事件回调实现（预留，后续完善具体逻辑）
void BattleScene::onMouseScroll2(EventMouse* event)
{
    // 预留，后续实现鼠标滚轮缩放逻辑
}
void BattleScene::onMouseDown2(EventMouse* event)
{
    CCLOG("Mouse Down Detected!"); //调试信息

    Vec2 mousePos = Vec2(event->getCursorX(), event->getCursorY());
    Vec2 worldPos = this->convertToNodeSpace(mousePos);
	spawnSoldierAtPosition(worldPos);

    // 预留，后续实现鼠标按下逻辑
}
void BattleScene::onMouseMove2(EventMouse* event)
{
    // 预留，后续实现鼠标移动逻辑
}
void BattleScene::onMouseUp2(EventMouse* event)
{
    // 预留，后续实现鼠标松开逻辑
}

// 实现 createScene 静态函数
Scene* BattleScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BattleScene::create();
    if (layer) {
        scene->addChild(layer);
    }
    return scene;
}
// 关闭战斗场景按钮点击回调实现
void BattleScene::closeBattleScene(Ref* sender)
{
    // 先关闭返回确认窗口
    closeReturnWindow(sender);
    // 使用popScene返回上一个场景（VillageScene），而非replaceScene
    // 这样会保留VillageScene的实例及其所有状态（包括已放置的建筑）
    // 停止背景音乐（双重保障，防止跳转时未停止）
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    Director::getInstance()->popScene();
}
// 关闭确认窗口按钮点击回调实现
void BattleScene::closeReturnWindow(Ref* sender)
{
    // 1. 删除菜单（包含Cancel/Confirm按钮）
    if (_returnMenu)
    {
        _returnMenu->removeFromParent(); // 从场景中移除
        _returnMenu = nullptr; // 置空避免野指针
    }

    // 2. 删除弹窗面板
    if (_returnPanel)
    {
        _returnPanel->removeFromParent(); // 从场景中移除
        _returnPanel = nullptr; // 置空避免野指针
    }
}
// 结束战斗按钮点击回调实现
void BattleScene::onReturnButtonClicked(Ref* sender)
{
    if (_isBattleStart)
        return;
	// 已经存在弹窗则不重复创建
    if (_returnPanel || _returnMenu)
        return;
    // 替换为成员变量：保存弹窗面板
    _returnPanel = cocos2d::Sprite::create("1Point_Return_Button.png");
    if (_returnPanel)
    {
        _returnPanel->setScale(1);
        _returnPanel->setPosition(cocos2d::Vec2(640, 360));
        this->addChild(_returnPanel, 30);

        auto CancleBtn = cocos2d::MenuItemImage::create(
            "Cancel_Button.png", "Cancel_Button.png",
            CC_CALLBACK_1(BattleScene::closeReturnWindow, this)
        );
        auto ConfirmBtn = cocos2d::MenuItemImage::create(
            "Confirm_Button.png", "Confirm_Button.png",
            CC_CALLBACK_1(BattleScene::closeBattleScene, this)
        );
        CancleBtn->setScale(0.5);
        ConfirmBtn->setScale(0.5);
        CancleBtn->setPosition(cocos2d::Vec2(520, 300));
        ConfirmBtn->setPosition(cocos2d::Vec2(760, 300));

        // 替换为成员变量：保存菜单（按钮在菜单内，删除菜单会自动删除按钮）
        _returnMenu = cocos2d::Menu::create(CancleBtn, ConfirmBtn, nullptr);
        _returnMenu->setPosition(cocos2d::Vec2::ZERO);
        this->addChild(_returnMenu, 31);
    }
}

void BattleScene::onSoldierSelectButtonClicked(Ref* sender)
{
    // 1. 转换点击的按钮对象
    auto clickedBtn = dynamic_cast<MenuItemImage*>(sender);
    if (!clickedBtn) 
        return;

    UnitType type = static_cast<UnitType>(clickedBtn->getTag());

    // 如果该兵种已经用完了，不允许选中
    if (ArmyManager::getInstance()->getRemainingCount(type) <= 0) return;

    // 2. 更新选中状态，标签和箭头位置
    _selectedSoldierBtn = clickedBtn;
	_selectedType = type;

    _arrowIndicator->setVisible(true);
    updateArrowPosition(clickedBtn);

    // 3. 预留：后续兵种选择逻辑（比如记录选中的兵种ID/类型）
    // 示例：
    // std::string soldierType = clickedBtn->getName(); // 可给按钮设置name标识兵种类型
    // _selectedSoldierType = soldierType;
}
// 封装：创建带小图标的士兵按钮（避免重复代码）
MenuItemImage* BattleScene::createSoldierButton(const Vec2& btnPos, const std::string& smallIconPath,UnitType type) {
    // 1. 创建基础按钮
    auto soldierBtn = MenuItemImage::create(
        "Soldier_Card.png",    // 正常状态
        "Soldier_Card1.png",   // 按下状态
		"Soldier_Card1.png", // 禁用状态(当兵种数量为0时)
        CC_CALLBACK_1(BattleScene::onSoldierSelectButtonClicked, this)
    );
    soldierBtn->setPosition(btnPos);
    soldierBtn->setTag(static_cast<int>(type)); // 用 Tag 绑定类型

    // 2. 创建小图片精灵（作为按钮的子节点）
    auto smallIcon = Sprite::create(smallIconPath); // 替换为你的小图片路径（如 "Small_Soldier_Icon.png"）
    if (smallIcon) { // 判空避免资源缺失崩溃
        // 设置小图片在按钮上的位置（示例：按钮中心，可根据需求调整偏移）
		smallIcon->setScale(1.5); // 可选：调整小图片大小以适应按钮
        smallIcon->setPosition(Vec2(
            soldierBtn->getContentSize().width / 2,   // 按钮宽度的一半（水平居中）
            soldierBtn->getContentSize().height / 2   // 按钮高度的一半（垂直居中）
        ));
        // 将小图片添加为按钮的子节点
        soldierBtn->addChild(smallIcon);
    }

    // 数量标签
    int count = ArmyManager::getInstance()->getRemainingCount(type);
    auto countLabel = Label::createWithTTF(std::to_string(count), "fonts/Marker Felt.ttf", 22);
    countLabel->setPosition(Vec2(soldierBtn->getContentSize().width / 2, 20));
    soldierBtn->addChild(countLabel);

    // 存入 Map 以便后续精准刷新
    _unitButtons[type] = soldierBtn;
    _unitLabels[type] = countLabel;

    return soldierBtn;
}

// 实现 init 初始化函数
bool BattleScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 加载配置与初始化兵力 
    ConfigManagerUnit::getInstance()->loadConfigs("data/units.json");

	// 模拟从村庄带来的兵力,到时候改成从VillageManager获取
    ArmyManager::getInstance()->setUnitCount(UnitType::MELEE, 50);
    ArmyManager::getInstance()->setUnitCount(UnitType::RANGED, 40);
    ArmyManager::getInstance()->setUnitCount(UnitType::TANK, 36);
    ArmyManager::getInstance()->setUnitCount(UnitType::WALL_BREAKER, 22);

    //  战斗背景（全屏铺开）
    auto background = Sprite::create("2village_background.jpg");
    if (background) {
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x,
            visibleSize.height / 2 + origin.y));
        this->addChild(background, GameConfig::Z_BACKGROUND);
    }

    // 绘制网格
    GridUtils::drawGrid(this);

    // 左下角 结束战斗按钮
    auto ReturnBtn = MenuItemImage::create(
        "Return_Button.png", "Return_Button.png",
        CC_CALLBACK_1(BattleScene::onReturnButtonClicked, this));
    ReturnBtn->setScale(0.5);
    ReturnBtn->setPosition(Vec2(50 + origin.x, 180 + origin.y));
    auto menu = Menu::create(ReturnBtn,nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 2); // UI层级高于背景


	//画一条水平线表示隔开兵种选择区和战斗区
    auto drawNode = cocos2d::DrawNode::create();
    this->addChild(drawNode, GameConfig::Z_UI);
	Vec2 startPoint = Vec2(0, 150);
	Vec2 endPoint = Vec2(visibleSize.width, 150);
    drawNode->drawLine(startPoint, endPoint, Color4F::WHITE);

	//兵种选择区预留（未实现）
    // 创建4个带小图标的士兵按钮
    auto Soldier1 = createSoldierButton(Vec2(200, 60),"Troop_HV_Barbarian_28.png" ,UnitType::MELEE);
    auto Soldier2 = createSoldierButton(Vec2(320, 60), "AQ_Japan_Neutral_Shadow_01.png",UnitType::RANGED);
    auto Soldier3 = createSoldierButton(Vec2(440, 60), "Giant_lvl_14.png",UnitType::TANK);
    auto Soldier4 = createSoldierButton(Vec2(560, 60), "Troop_HV_Wall_Breaker_1.png",UnitType::WALL_BREAKER);
    auto menu1 = Menu::create(Soldier1, Soldier2, Soldier3, Soldier4, nullptr);

    menu1->setPosition(Vec2::ZERO);
    this->addChild(menu1, GameConfig::Z_UI); // UI层级高于背景
    // ===================== 新增：初始化箭头指示器 =====================
    _arrowIndicator = Sprite::create("1arrow.png"); // 替换为你的箭头图片路径（如 "arrow_up.png"）
    if (_arrowIndicator) {
        _arrowIndicator->setScale(0.8f); // 根据实际图片大小调整缩放
        _arrowIndicator->setAnchorPoint(Vec2(0.5, 0)); // 箭头锚点设为底部中心，方便对齐按钮顶部
        _arrowIndicator->setVisible(true); // 初始显示
		this->addChild(_arrowIndicator, GameConfig::Z_UI + 1); // 层级高于士兵按钮，确保箭头在最上层

        // 默认选中第一个士兵按钮，箭头定位到第一个按钮上方
        _selectedSoldierBtn = Soldier1;
        updateArrowPosition(Soldier1);
    }

    // 注册事件
    registerMouseEvents2();

	//先删除背景音乐
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	// 播放战斗背景音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/battle_background.mp3", true);
    // 开启帧更新（后续战斗逻辑靠这个）
    this->scheduleUpdate();
    return true;
}

// 实现 update 帧更新函数
void BattleScene::update(float dt)
{
    // 暂时预留，后续添加：部队寻路、伤害结算、胜利判定等战斗逻辑
}

// 未实现的按钮回调（保持代码完整性）
void BattleScene::onSurrenderButtonClicked(Ref* sender) {}
void BattleScene::onNextWaveButtonClicked(Ref* sender) {}