// 必须包含对应的头文件
#include "BattleScene.h"
#include "Classes/System/ArmyManager.h"
#include "Classes/Entity/Unit/UnitFactory.h"
#include "Classes/System/ResourceManager.h"


gridinfo grid[41][41]; // 定义网格信息二维数组
// 初始化网格信息二维数组
void initializeGrid() {
    for (int i = 0; i < 41; i++) {
        for (int j = 0; j < 41; j++) {
            grid[i][j].init(); // 使用默认参数
        }
    }
}
//将建筑信息存入gridinfo二维数组(还没实现血量写入)
void BattleScene::initgridinfo(const int x, const int y, int buildingtype)
{
    grid[x][y].buildingtype = buildingtype;
    /*grid[x + 1][y].buildingtype = buildingtype;
    grid[x + 1][y + 1].buildingtype = buildingtype;
    grid[x + 1][y - 1].buildingtype = buildingtype;
    grid[x][y + 1].buildingtype = buildingtype;
    grid[x][y - 1].buildingtype = buildingtype;
    grid[x - 1][y].buildingtype = buildingtype;
    grid[x - 1][y + 1].buildingtype = buildingtype;
    grid[x - 1][y - 1].buildingtype = buildingtype;*/
    //周围8个格子也标记为有建筑(暂存),好像没必要都标记
	grid[x][y].max_health = 1000; // 最大血量(暂时先写死，后面再调整)
	grid[x][y].now_health = 1000; // 现在血量(暂时先写死，后面再调整)
}
// 战斗结束实现(战斗结束后调用)
void BattleScene::battleOver()
{
    // 如果已经显示了，或者正在显示中，就不要再 addChild 了
    if (_isBattleOver || resultUIContainer->getParent() != nullptr) {
        return;
    }

	int countdestroyed = 0;
	//遍历gridinfo二维数组，检查建筑状态,判断胜负
    for (int i = 1; i <= 40; i++)
    {
        for(int j = 1; j <= 40; j++)
        {
            if(grid[i][j].now_health == -1)//-1的血量表示被摧毁
            {
				countdestroyed++;//记录建筑被摧毁数量
                if (grid[i][j].buildingtype == 1)
                {
                    //大本营被摧毁
                    _iswin = true;
                }
			}
		}
    }
	//如果摧毁建筑数量达到一定比例，判定为胜利
    if (countdestroyed >= 9) // 摧毁9个建筑(超过一半)算胜利
    {
        _iswin = true;
    }
    this->addChild(resultUIContainer, 100); // 战斗结果UI添加到场景
    auto bg = Sprite::create();
    bg->setTextureRect(Rect(0, 0, 1280,720));
    bg->setColor(Color3B::GRAY);
    bg->setOpacity(220);
    bg->setPosition(640, 360);
    resultUIContainer->addChild(bg,100);
    // 显示战斗结果UI等
    if (_iswin)
    {
        auto victory = Sprite::create("victory.png");
        victory->setScale(0.8);
        victory->setPosition(640, 550);
        resultUIContainer->addChild(victory, 101);
		auto goldpic = Sprite::create("coin.png");
        goldpic->setScale(0.2);
        goldpic->setPosition(700, 420);
        resultUIContainer->addChild(goldpic, 101);
		auto goldlabel = Label::createWithTTF("+1000", "fonts/Marker Felt.ttf", 30);
		goldlabel->setColor(Color3B::BLACK);
		goldlabel->setPosition(600, 420);
        resultUIContainer->addChild(goldlabel, 101);
		auto elixirpic = Sprite::create("elixir.png");
		elixirpic->setScale(0.2);
		elixirpic->setPosition(700, 370);
        resultUIContainer->addChild(elixirpic, 101);
		auto elixirlabel = Label::createWithTTF("+1000", "fonts/Marker Felt.ttf", 30);
		elixirlabel->setColor(Color3B::BLACK);
		elixirlabel->setPosition(600, 370);
        resultUIContainer->addChild(elixirlabel, 101);

        ResourceManager::getInstance()->addElixir(1000);
		ResourceManager::getInstance()->addGold(1000);

        CCLOG("Battle Won!");
        // 处理胜利逻辑
    }
    else
    {
		auto defeat = Sprite::create("defeat.png");
        defeat->setScale(0.8);
		defeat->setPosition(640, 550);
        resultUIContainer->addChild(defeat, 101);
        auto goldpic = Sprite::create("coin.png");
        goldpic->setScale(0.2);
        goldpic->setPosition(700, 420);
        resultUIContainer->addChild(goldpic, 101);
        auto goldlabel = Label::createWithTTF("+0", "fonts/Marker Felt.ttf", 30);
        goldlabel->setColor(Color3B::BLACK);
        goldlabel->setPosition(600, 420);
        resultUIContainer->addChild(goldlabel, 101);
        auto elixirpic = Sprite::create("elixir.png");
        elixirpic->setScale(0.2);
        elixirpic->setPosition(700, 370);
        resultUIContainer->addChild(elixirpic, 101);
        auto elixirlabel = Label::createWithTTF("+0", "fonts/Marker Felt.ttf", 30);
        elixirlabel->setColor(Color3B::BLACK);
        elixirlabel->setPosition(600, 370);
        resultUIContainer->addChild(elixirlabel, 101);
        CCLOG("Battle Lost!");
        // 处理失败逻辑
    }
    auto Btn_Confirm = MenuItemImage::create(
        "Confirm_Button.png",
        "Confirm_Button.png",
        CC_CALLBACK_1(BattleScene::onBtn_ConfirmClicked, this));
    Btn_Confirm->setScale(0.6);
    Btn_Confirm->setPosition(Vec2(640, 250));
    auto menu = Menu::create(Btn_Confirm, nullptr);
    menu->setPosition(Vec2::ZERO);
    resultUIContainer->addChild(menu, 101);
}
//实现确认按钮点击回调
void BattleScene::onBtn_ConfirmClicked(Ref* sender)
{
    SimpleAudioEngine::getInstance()->playEffect("audio/button_click.mp3");
    // 清理战斗场景，返回村庄场景
    if(resultUIContainer)
    {
        this->removeAllChildren();
        resultUIContainer = nullptr;
    }
	void getRemainingCapacity(); // 获取剩余兵力
    // 停止背景音乐（双重保障，防止跳转时未停止）
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    Director::getInstance()->popScene();
}
// 建筑生成实现
void BattleScene::spawnBuilding(Vec2 gridPos, int Buildingname)
{
    int x = static_cast<int>(gridPos.x);
    int y = static_cast<int>(gridPos.y);
	// 根据Buildingname选择不同建筑类型
    switch (Buildingname)
    {
        case 0:
        {
            realSprite = Sprite::create("town_hall_lv3.png");
            initgridinfo(x, y, 1); // 大本营建筑类型为1
            break;
        }
        case 1:
        {
            realSprite = Sprite::create("gold_mine_lv2.png");
            initgridinfo(x, y, 2); // 普通建筑类型为2
            break;
        }
        case 2:
        {
            realSprite = Sprite::create("elixir_collector_lv2.png");
            initgridinfo(x, y, 2); // 普通建筑类型为2
            break;
        }
        case 3:
        {
            realSprite = Sprite::create("gold_storage_lv2.png");
            initgridinfo(x, y, 2); // 普通建筑类型为2
            break;
        }
        case 4:
        {
            realSprite = Sprite::create("elixir_storage_lv3.png");
            initgridinfo(x, y, 2); // 普通建筑类型为2
            break;
        }
        case 5:
        {
            realSprite = Sprite::create("military_camp_lv3.png");
            initgridinfo(x, y, 2); // 普通建筑类型为2
            break;
        }
        case 6:
        {
            realSprite = Sprite::create("archer_tower_lv2.png");
            initgridinfo(x, y, 4); // 箭塔为4
            break;
        }
        case 7:
        {
            realSprite = Sprite::create("canon_lv2.png");
            initgridinfo(x, y, 3); // 加农炮为3
            break;
        }
        case 8:
            realSprite = Sprite::create("worker_home.png");
            initgridinfo(x, y, 2); // 普通建筑类型为2
            break;
        default:
            break;
            // 实现建筑初始化
    }
    Vec2 finalPos = GridUtils::gridToWorld(gridPos);
    realSprite->setAnchorPoint(Vec2(0.5, 0.5));
    realSprite->setPosition(finalPos);
    this->addChild(realSprite, 5);
}


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
    if (!_isBattleStart)
        return;
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

            // 因为 Unit 类不是 Node，不会自动随场景销毁
            // 我们需要手动引用计数并加入列表
            newUnit->retain();
            _liveUnits.push_back(newUnit);

        }

        // 4. 刷新 UI
        refreshUI();
    }
}

void BattleScene::refreshUI() {
	for (auto const& [type, btn] : _unitButtons) {//我这里用了C++17的结构化绑定，现代特性
        int count = ArmyManager::getInstance()->getRemainingCount(type)/2;

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
    // 鼠标按下事件
    auto mouseDownListener = EventListenerMouse::create();
    mouseDownListener->onMouseDown = CC_CALLBACK_1(BattleScene::onMouseDown2, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseDownListener, this);
}
void BattleScene::onMouseDown2(EventMouse* event)
{
    CCLOG("Mouse Down Detected!"); //调试信息

    Vec2 mousePos = Vec2(event->getCursorX(), event->getCursorY());
    Vec2 worldPos = this->convertToNodeSpace(mousePos);
	spawnSoldierAtPosition(worldPos);

    // 预留，后续实现鼠标按下逻辑
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
    SimpleAudioEngine::getInstance()->playEffect("audio/button_click.mp3");
    // 先关闭返回确认窗口
    closeReturnWindow(sender);
	_isBattleStart = false;// 标记战斗未开始
    // 使用popScene返回上一个场景（VillageScene），而非replaceScene
    // 这样会保留VillageScene的实例及其所有状态（包括已放置的建筑）
    // 停止背景音乐（双重保障，防止跳转时未停止）
    ArmyManager::getInstance()->getRemainingCapacity(); // 获取剩余兵力
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    Director::getInstance()->popScene();
}
// 关闭确认窗口按钮点击回调实现
void BattleScene::closeReturnWindow(Ref* sender)
{
    _isBattleStart = true;
    SimpleAudioEngine::getInstance()->playEffect("audio/button_click.mp3");
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
    _isBattleStart = false;
	// 已经存在弹窗则不重复创建
    if (_returnPanel || _returnMenu)
        return;
    SimpleAudioEngine::getInstance()->playEffect("audio/button_click.mp3");
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
    SimpleAudioEngine::getInstance()->playEffect("audio/button_click.mp3");
    // 1. 转换点击的按钮对象
    _isBattleStart = true;
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
    int count = ArmyManager::getInstance()->getRemainingCount(type)/2;
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
	initializeGrid(); // 初始化网格信息二维数组
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 加载配置与初始化兵力 
    ConfigManagerUnit::getInstance()->loadConfigs("data/units.json");

	// 拿到从村庄带来的兵力
    ArmyManager::getInstance()->initializeArmyForBattle();
    
    //  战斗背景（全屏铺开）
    auto background = Sprite::create("battle_background.jpg");
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
    // 预加载音效文件
    SimpleAudioEngine::getInstance()->preloadEffect("audio/button_click.mp3"); 
    //初始化生成建筑
    /*0:大本营 1:金矿 2:圣水收集器 3:金库 4:圣水库 5:军营 6:箭塔 7:加农炮 8:建筑工人
    */
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 1);
	const int mode = dis(gen); //随机选择一种布局模式
    if(mode==0)
    {
        // 核心：大本营（唯一，3x3，中心位置）
        spawnBuilding(Vec2(20, 20), 0);

        // 资源生产（多个，紧邻大本营，对称分布）
        spawnBuilding(Vec2(16, 20), 1); // 金矿1（大本营左侧）
        spawnBuilding(Vec2(24, 20), 1); // 金矿2（大本营右侧）
        spawnBuilding(Vec2(20, 16), 2); // 圣水收集器1（大本营下侧）
        spawnBuilding(Vec2(20, 24), 2); // 圣水收集器2（大本营上侧）

        // 资源存储（多个，紧邻生产建筑，防御圈内）
        spawnBuilding(Vec2(16, 16), 3); // 金库1（金矿1+圣水收集器1旁）
        spawnBuilding(Vec2(24, 24), 3); // 金库2（金矿2+圣水收集器2旁）
        spawnBuilding(Vec2(16, 24), 4); // 圣水库1（金矿1+圣水收集器2旁）
        spawnBuilding(Vec2(24, 16), 4); // 圣水库2（金矿2+圣水收集器1旁）

        // 军事生产（多个，核心外侧，方便造兵）
        spawnBuilding(Vec2(13, 20), 5); // 军营1（左侧外围）
        spawnBuilding(Vec2(27, 20), 5); // 军营2（右侧外围）

        // 防御建筑（对称分布，覆盖核心+资源）
        spawnBuilding(Vec2(13, 13), 6); // 箭塔1（左下防御，覆盖金库1）
        spawnBuilding(Vec2(27, 27), 6); // 箭塔2（右上防御，覆盖金库2）
        spawnBuilding(Vec2(13, 27), 7); // 加农炮1（左上防御，覆盖圣水库1）
        spawnBuilding(Vec2(27, 13), 7); // 加农炮2（右下防御，覆盖圣水库2）

        // 功能建筑（最外侧，不占用核心）
        spawnBuilding(Vec2(10, 20), 8); // 建筑工人小屋1（最左侧）
        spawnBuilding(Vec2(30, 20), 8); // 建筑工人小屋2（最右侧）
    }
    else
    {
        // 核心：大本营（唯一，偏右但不极端，x=24更合理，核心区x=12~30）
        spawnBuilding(Vec2(24, 20), 0);

        // 资源生产（3金+3水，分布松散不拥挤，左区x=8~18，避免极端边缘）
        spawnBuilding(Vec2(12, 20), 1);  // 金矿1（左区核心，大本营正左）
        spawnBuilding(Vec2(12, 16), 1);  // 金矿2（左区下侧，不重叠）
        spawnBuilding(Vec2(12, 24), 1);  // 金矿3（左区上侧，对称）
        spawnBuilding(Vec2(18, 20), 2);  // 圣水收集器1（金矿1右侧，近大本营）
        spawnBuilding(Vec2(18, 16), 2);  // 圣水收集器2（金矿2右侧，对称）
        spawnBuilding(Vec2(18, 24), 2);  // 圣水收集器3（金矿3右侧，对称）

        // 资源存储（2金+2水，数量适配产出，精准贴靠生产建筑）
        spawnBuilding(Vec2(8, 20), 3);   // 金库1（金矿1左侧，左区边缘）
        spawnBuilding(Vec2(8, 16), 3);   // 金库2（金矿2左侧，对称）
        spawnBuilding(Vec2(8, 24), 4);   // 圣水库1（金矿3左侧，对称）

        // 军事生产（仅1个，大本营正下侧，核心防御区）
        spawnBuilding(Vec2(24, 16), 5);  // 军营1（大本营下，造兵后快速支援）

        // 防御建筑（精准覆盖，不浪费边缘，每个防御至少覆盖2个资源+大本营）
        spawnBuilding(Vec2(8, 12), 6);    // 箭塔1（左下，覆盖金库2+金矿2）
        spawnBuilding(Vec2(8, 28), 6);    // 箭塔2（左上，覆盖圣水库1+金矿3）
        spawnBuilding(Vec2(30, 16), 7);   // 加农炮1（右下，覆盖军营+大本营右侧）
        spawnBuilding(Vec2(30, 24), 7);   // 加农炮2（右上，覆盖大本营上侧+圣水收集器3）

        // 功能建筑（仅1个，左区资源旁，方便升级资源建筑，不极端）
        spawnBuilding(Vec2(3, 20), 8);    // 建筑工人小屋1（左区核心旁，操作便捷）
    }

    // 初始化防御计时器为 0
    for (int i = 0; i < 41; ++i)
        for (int j = 0; j < 41; ++j)
            _defenseTimers[i][j] = 0.0f;



    // 开启帧更新（后续战斗逻辑靠这个）
    this->scheduleUpdate();
    return true;
}

//控制建筑攻击逻辑
void BattleScene::updateDefenseBuildings(float dt) 
{
    // 定义防御建筑的数值（为了简单直接写死，也可以去查 BuildingData）
    float cannonRange = 250.0f;    // 加农炮射程
    float cannonDamage = 20.0f;    // 每次伤害
    float cannonInterval = 1.5f;   // 攻击间隔

    float archerRange = 350.0f;    // 箭塔射程更远
    float archerDamage = 12.0f;    // 伤害稍低
    float archerInterval = 0.8f;   // 攻速更快

    for (int i = 1; i <= 40; i++) {
        for (int j = 1; j <= 40; j++) {
            int type = grid[i][j].buildingtype;
            if ((type == 3 || type == 4) && grid[i][j].now_health > 0) {

                // 1. 更新冷却时间
                _defenseTimers[i][j] += dt;

                float interval = (type == 3) ? cannonInterval : archerInterval;
                if (_defenseTimers[i][j] >= interval) {

                    // 2. 寻找最近的敌人
                    Vec2 buildPos = GridUtils::gridToWorld(Vec2(i, j));
                    Unit* targetUnit = nullptr;
                    float minDistance = (type == 3) ? cannonRange : archerRange;

                    for (auto unit : _liveUnits) {
                        if (!unit->isDead()) {
                            float d = buildPos.distance(unit->getPosition());
                            if (d < minDistance) {
                                minDistance = d;
                                targetUnit = unit;
                            }
                        }
                    }

                    // 3. 如果找到敌人，发射子弹
                    if (targetUnit) {
                        _defenseTimers[i][j] = 0.0f; // 重置冷却

                        std::string img = (type == 3) ? "cannon_ball.png" : "arrow.png";
                        float damage = (type == 3) ? cannonDamage : archerDamage;

                        // 创建子弹表现
                        auto bullet = Sprite::create(img);
                        bullet->setPosition(buildPos);
                        this->addChild(bullet, GameConfig::Z_EFFECT);

                        // 子弹飞行并造成伤害
                        float flyTime = buildPos.distance(targetUnit->getPosition()) / 600.0f;
                        auto moveTo = MoveTo::create(flyTime, targetUnit->getPosition());
                        auto hit = CallFunc::create([targetUnit, damage, bullet]() {
                            if (targetUnit && !targetUnit->isDead()) {
                                targetUnit->takeDamage(damage);
                            }
                            bullet->removeFromParent();
                            });
                        bullet->runAction(Sequence::create(moveTo, hit, nullptr));
                    }
                }
            }
        }
    }
}

// 实现 update 帧更新函数
void BattleScene::update(float dt)
{

    // 启动防御建筑逻辑
    updateDefenseBuildings(dt);

    // 遍历所有活着的兵
    for (auto it = _liveUnits.begin(); it != _liveUnits.end(); ) {
        Unit* unit = *it;

        if (unit->isDead()) {
            // 如果兵死了：
            unit->release();        // 释放内存
            it = _liveUnits.erase(it); // 从列表中移除
        }
        else {
            // 如果兵活着：执行它的大脑逻辑（寻敌、移动、攻击）
            unit->update(dt);
            ++it;
        }
    }

    // 检查 A：是否还有活着的建筑
    bool hasBuildingAlive = false;
    for (int i = 1; i <= 40; i++) {
        for (int j = 1; j <= 40; j++) {
            if (grid[i][j].buildingtype > 0 && grid[i][j].now_health > 0) {
                hasBuildingAlive = true;
                break;
            }
        }
        if (hasBuildingAlive) break;
    }

    // 检查 B：玩家是否还能出兵 (遍历四种兵种)
    bool canStillSpawn = false;
    for (int typeID = 0; typeID <= 3; typeID++) {
        if (ArmyManager::getInstance()->getRemainingCount(static_cast<UnitType>(typeID)) > 0) {
            canStillSpawn = true;
            break;
        }
    }

    // --- 决定是否吹哨 ---

    // 情况 1: 100% 拆迁完成
    if (!hasBuildingAlive) {
        _isBattleOver = true; // 防止重复调用
        this->battleOver();   // 交给原本的函数去处理结算和 UI
        return;
    }

    // 情况 2: 场上没兵了 且 手里也没兵了
    if (_liveUnits.empty() && !canStillSpawn) {
        // 稍微给一点延迟（比如1秒），让最后一发炮弹飞一会儿，视觉上更自然
        _isBattleOver = true;
        this->runAction(Sequence::create(
            DelayTime::create(1.0f),
            CallFunc::create([this]() { this->battleOver(); }),
            nullptr
        ));
    }

}

// 未实现的按钮回调（保持代码完整性）
void BattleScene::onSurrenderButtonClicked(Ref* sender) {}
void BattleScene::onNextWaveButtonClicked(Ref* sender) {}