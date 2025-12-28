#include "StoreWindow.h"
#include "cocos2d.h"
#include "Classes/Core/GameDirector.h"
#include "Classes/System/ResourceManager.h"

StoreWindow* StoreWindow::create(const std::function<void(BuildingType)>& placeCallback)
{
    auto window = new (std::nothrow) StoreWindow();

    if (window && window->init() && window->initWithPlaceCallback(placeCallback))
    {
        window->autorelease();
        return window;
    }

    CC_SAFE_DELETE(window);
    return nullptr;
}

bool StoreWindow::initWithPlaceCallback(const std::function<void(BuildingType)>& placeCallback)
{
    _placeCallback = placeCallback;
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();

    // 遮罩层
    _grayMask = LayerColor::create(Color4B(0, 0, 0, 180), _visibleSize.width, _visibleSize.height);
    _grayMask->setPosition(_origin);
    _grayMask->setVisible(false);
    this->addChild(_grayMask, 10);

    // 主面板
    _storePanel = Sprite::create("basic_market_bar.png");
    if (_storePanel)
    {
        _storePanel->setPosition(Vec2(_visibleSize.width / 2 + _origin.x, _visibleSize.height / 2 + _origin.y));
        _storePanel->setVisible(false);
        this->addChild(_storePanel, 20);

        // 关闭按钮（右上）
        auto closeBtn = MenuItemImage::create("out_of_now.png", "out_of_now.png",
            CC_CALLBACK_1(StoreWindow::onCloseButtonClicked, this));
        closeBtn->setPosition(Vec2(_storePanel->getContentSize().width - 40,
            _storePanel->getContentSize().height - 40));

        // 九个建筑按钮
        std::vector<BuildingType> types =
        {
            BuildingType::ARCHER_TOWER, BuildingType::CANNON, BuildingType::ELIXIR_COLLECTOR,
            BuildingType::ELIXIR_STORAGE, BuildingType::GOLD_MINE,
            BuildingType::GOLD_STORAGE, BuildingType::MILITARY_CAMP, BuildingType::TOWN_HALL,
            BuildingType::WORKER_HOME
        };

        // 布局：上排5个，下排4个（假设面板宽高足够）
        float panelW = _storePanel->getContentSize().width;
        float panelH = _storePanel->getContentSize().height;
        float startX = 100;           // 左边距
        float startY = panelH - 190;  // 上边距
        float spacingX = (panelW - 200) / 4;  // 5个按钮分4份间隙
        float spacingY = 240;

        Vector<MenuItem*> items;
        items.pushBack(closeBtn);

        for (int i = 0; i < 9; ++i)
        {
            BuildingType type = types[i];
            std::string iconName;
            switch (type)
            {
                case BuildingType::TOWN_HALL:
                    iconName = "town_hall_lv1_sub.png";
                    break;

                case BuildingType::GOLD_MINE: 
                    iconName = "gold_mine_lv1_sub.png";
                    break;

                case BuildingType::ELIXIR_COLLECTOR:
                    iconName = "elixir_collector_lv1_sub.png"; 
                    break;

                case BuildingType::GOLD_STORAGE: 
                    iconName = "gold_storage_lv1_sub.png";
                    break;

                case BuildingType::ELIXIR_STORAGE:
                    iconName = "elixir_storage_lv1_sub.png"; 
                    break;

                case BuildingType::MILITARY_CAMP:
                    iconName = "military_camp_lv1_sub.png";
                    break;

                case BuildingType::ARCHER_TOWER:
                    iconName = "archer_tower_lv1_sub.png";
                    break;

                case BuildingType::CANNON:
                    iconName = "canon_lv1_sub.png";
                    break;

                case BuildingType::WORKER_HOME:
                    iconName = "worker_home_lv1_sub.png"; 
                    break;
            }

            // 判断可用
            std::string barName;

            auto rm = ResourceManager::getInstance();

            bool available = getCurrentCount(type) < getMaxCount(type, rm->getTownHallLevel());

            if (type == BuildingType::GOLD_MINE || type == BuildingType::GOLD_STORAGE || type == BuildingType::CANNON)
            {
                // 圣水收集器和圣水库用紫色底栏
                barName = available ? "elixir_bar_for_available_building_in_store.png"
                    : "elixir_bar_for_inavailable_building_in_store.png";
            }

            else
            {
                // 金矿和金库用金色底栏
                barName = available ? "gold_bar_for_available_building_in_store.png"
                    : "gold_bar_for_inavailable_building_in_store.png";
			}

            // 融合图标+底栏（用Node包一层）
            auto buttonNode = Node::create();
            auto icon = Sprite::create(iconName);
            auto bar = Sprite::create(barName);
            if (icon && bar) 
            {
                icon->setPosition(Vec2(bar->getContentSize().width / 2,
                    bar->getContentSize().height / 2 + 20));  // 图标在上
                bar->setPosition(Vec2(bar->getContentSize().width / 2, bar->getContentSize().height / 2));
                buttonNode->addChild(bar, 0);
                buttonNode->addChild(icon, 1);
                buttonNode->setContentSize(bar->getContentSize());

                // ========== 新增：显示建造花费 ==========
                int goldCost = 0;
                int elixirCost = 0;

                // 根据 BuildingType 获取对应数据
                if (type == BuildingType::TOWN_HALL) 
                {
                    goldCost = TownHallBuildingData.goldCost[0];
                }

                else if (type == BuildingType::GOLD_MINE || type == BuildingType::GOLD_STORAGE) 
                {
                    const auto& data = (type == BuildingType::GOLD_MINE) ? GoldMineBuildingData : GoldStorageBuildingData;
                    elixirCost = data.elixirCost[0];
                }

                else if (type == BuildingType::ELIXIR_COLLECTOR || type == BuildingType::ELIXIR_STORAGE) 
                {
                    const auto& data = (type == BuildingType::ELIXIR_COLLECTOR) ? ElixirCollectorBuildingData : ElixirStorageBuildingData;
                    goldCost = data.goldCost[0];
                }

                else if (type == BuildingType::MILITARY_CAMP)
                {
                    goldCost = MilitaryBuildingBuildingData.goldCost[0];
                }

                else if (type == BuildingType::ARCHER_TOWER) 
                {
                    goldCost = ArcherTowerBuildingData.goldCost[0];
                }

                else if (type == BuildingType::CANNON) 
                {
                    elixirCost = CanonBuildingData.elixirCost[0];
                }

                else if (type == BuildingType::WORKER_HOME)
                {
                    goldCost = WorkerHomeBuildingData.goldCost[0];
                }

                // 创建花费文本
                std::string costText = "";
                if (goldCost > 0) 
                {
                    costText = std::to_string(goldCost);
                }

                else
                {
                    costText = std::to_string(elixirCost);
                }

                if (!costText.empty()) 
                {
                    auto costLabel = Label::createWithSystemFont(costText, "Arial", 20);
                    costLabel->setTextColor(Color4B::WHITE);
                    costLabel->enableOutline(Color4B::BLACK, 2);

                    // 设置颜色：如果只花金币 → 金色；只花圣水 → 紫色；两者都有 → 白色+图标区分（这里简化为白色）
                    if (goldCost > 0 && elixirCost == 0) 
                    {
                        costLabel->setTextColor(Color4B(255, 215, 0, 255)); // 金色
                    }

                    else if (elixirCost > 0 && goldCost == 0)
                    {
                        costLabel->setTextColor(Color4B(186, 85, 211, 255)); // 紫色（圣水色）
                    }

                    // 放在底栏下方
                    float barHeight = bar->getContentSize().height;
                    costLabel->setPosition(Vec2(bar->getContentSize().width / 2, -barHeight / 2 + 150));
                    buttonNode->addChild(costLabel, 3);

                    auto rmw = ResourceManager::getInstance();

                    auto alreadyHaveLabel = Label::createWithSystemFont(std::to_string(rmw->getBuildingCount(StoreWindow::BuildingType::TOWN_HALL)), "arial", 20);
                    alreadyHaveLabel->setTextColor(Color4B::WHITE);
                    alreadyHaveLabel->enableOutline(Color4B::BLACK, 2);

                    alreadyHaveLabel->setPosition(Vec2(bar->getContentSize().width / 2, -barHeight / 2 + 160));
                    buttonNode->addChild(alreadyHaveLabel, 3);
                }
                // ======================================
            }

            // 计算位置
            int row = i / 5;  // 0或1
            int col = i % 5;
            if (row == 1) col += 0.5f;  // 下排居中偏移
            float x = startX + col * spacingX;
            float y = startY - row * spacingY;

            auto btn = MenuItemSprite::create(buttonNode, buttonNode, buttonNode,
                [this, type, available](Ref*) 
                {
                    if (available) 
                    {
                        this->hide();
                        if (_placeCallback) _placeCallback(type);
                    }

                    else 
                    {
                        // 不可用时晃晃提示（可选）
                        // todo: 播放不可用动画
                    }
                });

            btn->setPosition(Vec2(x, y));
            items.pushBack(btn);
        }

        _menu = Menu::create();

        _menu->setPosition(Vec2::ZERO);

        for (auto item : items)
        {
            _menu->addChild(item);
        }
        _storePanel->addChild(_menu, 1);
    }

    return true;
}

void StoreWindow::show()
{
    this->setVisible(true);
    if (_grayMask) _grayMask->setVisible(true);
    if (_storePanel) _storePanel->setVisible(true);
    // === 重建菜单 ===
    rebuildMenu();
    // ==============
}

void StoreWindow::rebuildMenu() 
{
    // 移除旧菜单
    if (_menu) 
    {
        _storePanel->removeChild(_menu, true);
    }

    // 重新创建菜单（复用 initWithPlaceCallback 的逻辑）
    auto closeBtn = MenuItemImage::create("out_of_now.png", "out_of_now.png",
        CC_CALLBACK_1(StoreWindow::onCloseButtonClicked, this));
    closeBtn->setPosition(Vec2(_storePanel->getContentSize().width - 40,
                              _storePanel->getContentSize().height - 40));

    Vector<MenuItem*> items;
    items.pushBack(closeBtn);

    // 九个建筑按钮
    std::vector<BuildingType> types = 
    {
        BuildingType::ARCHER_TOWER, BuildingType::CANNON, BuildingType::ELIXIR_COLLECTOR,
        BuildingType::ELIXIR_STORAGE, BuildingType::GOLD_MINE,
        BuildingType::GOLD_STORAGE, BuildingType::MILITARY_CAMP, BuildingType::TOWN_HALL,
        BuildingType::WORKER_HOME
    };

    // 布局：上排5个，下排4个（假设面板宽高足够）
    float panelW = _storePanel->getContentSize().width;
    float panelH = _storePanel->getContentSize().height;
    float startX = 100;           // 左边距
    float startY = panelH - 190;  // 上边距
    float spacingX = (panelW - 200) / 4;  // 5个按钮分4份间隙
    float spacingY = 240;

    for (int i = 0; i < 9; ++i)
    {
        BuildingType type = types[i];
        std::string iconName;
        switch (type)
        {
            case BuildingType::TOWN_HALL: 
                iconName = "town_hall_lv1_sub.png"; 
                break;

            case BuildingType::GOLD_MINE: 
                iconName = "gold_mine_lv1_sub.png"; 
                break;

            case BuildingType::ELIXIR_COLLECTOR: 
                iconName = "elixir_collector_lv1_sub.png"; 
                break;

            case BuildingType::GOLD_STORAGE: 
                iconName = "gold_storage_lv1_sub.png"; 
                break;

            case BuildingType::ELIXIR_STORAGE: 
                iconName = "elixir_storage_lv1_sub.png";
                break;

            case BuildingType::MILITARY_CAMP: 
                iconName = "military_camp_lv1_sub.png";
                break;

            case BuildingType::ARCHER_TOWER: 
                iconName = "archer_tower_lv1_sub.png"; 
                break;

            case BuildingType::CANNON: 
                iconName = "canon_lv1_sub.png"; 
                break;

            case BuildingType::WORKER_HOME:
                iconName = "worker_home_lv1_sub.png"; 
                break;
        }

        // 判断可用
        std::string barName;

        auto rm = ResourceManager::getInstance();

        bool available = getCurrentCount(type) < getMaxCount(type, rm->getTownHallLevel());

        if (type == BuildingType::GOLD_MINE || type == BuildingType::GOLD_STORAGE || type == BuildingType::CANNON)
        {
            // 圣水收集器和圣水库用紫色底栏
            barName = available ? "elixir_bar_for_available_building_in_store.png"
                : "elixir_bar_for_inavailable_building_in_store.png";
        }

        else
        {
            // 金矿和金库用金色底栏
            barName = available ? "gold_bar_for_available_building_in_store.png"
                : "gold_bar_for_inavailable_building_in_store.png";
        }

        // 融合图标+底栏（用Node包一层）
        auto buttonNode = Node::create();
        auto icon = Sprite::create(iconName);
        auto bar = Sprite::create(barName);
        if (icon && bar) 
        {
            icon->setPosition(Vec2(bar->getContentSize().width / 2,
                bar->getContentSize().height / 2 + 20));  // 图标在上
            bar->setPosition(Vec2(bar->getContentSize().width / 2, bar->getContentSize().height / 2));
            buttonNode->addChild(bar, 0);
            buttonNode->addChild(icon, 1);
            buttonNode->setContentSize(bar->getContentSize());

            // ========== 新增：显示建造花费 ==========
            int goldCost = 0;
            int elixirCost = 0;

            // 根据 BuildingType 获取对应数据
            if (type == BuildingType::TOWN_HALL)
            {
                goldCost = TownHallBuildingData.goldCost[0];
            }

            else if (type == BuildingType::GOLD_MINE || type == BuildingType::GOLD_STORAGE)
            {
                const auto& data = (type == BuildingType::GOLD_MINE) ? GoldMineBuildingData : GoldStorageBuildingData;
                elixirCost = data.elixirCost[0];
            }

            else if (type == BuildingType::ELIXIR_COLLECTOR || type == BuildingType::ELIXIR_STORAGE)
            {
                const auto& data = (type == BuildingType::ELIXIR_COLLECTOR) ? ElixirCollectorBuildingData : ElixirStorageBuildingData;
                goldCost = data.goldCost[0];
            }

            else if (type == BuildingType::MILITARY_CAMP) 
            {
				goldCost = MilitaryBuildingBuildingData.goldCost[0];
            }

            else if (type == BuildingType::ARCHER_TOWER)
            {
                goldCost = ArcherTowerBuildingData.goldCost[0];
            }

            else if (type == BuildingType::CANNON)
            {
                elixirCost = CanonBuildingData.elixirCost[0];
            }

            else if (type == BuildingType::WORKER_HOME) 
            {
                goldCost = WorkerHomeBuildingData.goldCost[0];
            }

            // 创建花费文本
            std::string costText = "";
            if (goldCost > 0)
            {
                costText = std::to_string(goldCost);
            }

            else
            {
                costText = std::to_string(elixirCost);
            }

            if (!costText.empty())
            {
                auto costLabel = Label::createWithSystemFont(costText, "Arial", 20);
                costLabel->setTextColor(Color4B::WHITE);
                costLabel->enableOutline(Color4B::BLACK, 2);

                // 设置颜色：如果只花金币 → 金色；只花圣水 → 紫色；两者都有 → 白色+图标区分（这里简化为白色）
                if (goldCost > 0 && elixirCost == 0)
                {
                    costLabel->setTextColor(Color4B(255, 215, 0, 255)); // 金色
                }

                else if (elixirCost > 0 && goldCost == 0) 
                {
                    costLabel->setTextColor(Color4B(186, 85, 211, 255)); // 紫色（圣水色）
                }

                // 放在底栏下方
                float barHeight = bar->getContentSize().height;
                costLabel->setPosition(Vec2(bar->getContentSize().width / 2, -barHeight / 2 + 150));
                buttonNode->addChild(costLabel, 3);

                auto rmw = ResourceManager::getInstance();

                std::string have = std::to_string(rmw->getBuildingCount(type)) + '/' + std::to_string(getMaxCount(type, rm->getTownHallLevel()));

                auto alreadyHaveLabel = Label::createWithSystemFont(have, "arial", 20);
                alreadyHaveLabel->setTextColor(Color4B::WHITE);
                alreadyHaveLabel->enableOutline(Color4B::BLACK, 2);

                alreadyHaveLabel->setPosition(Vec2(bar->getContentSize().width / 2, -barHeight / 2 + 180));
                buttonNode->addChild(alreadyHaveLabel, 3);
            }
            // ======================================
        }

        // 计算位置
        int row = i / 5;  // 0或1
        int col = i % 5;
        if (row == 1) col += 0.5f;  // 下排居中偏移
        float x = startX + col * spacingX;
        float y = startY - row * spacingY;

        auto btn = MenuItemSprite::create(buttonNode, buttonNode, buttonNode,
            [this, type, available](Ref*)
            {
                if (available) 
                {
                    this->hide();
                    if (_placeCallback) _placeCallback(type);
                }

                else 
                {
                    // 不可用时晃晃提示（可选）
                    // todo: 播放不可用动画
                }
            });

        btn->setPosition(Vec2(x, y));
        items.pushBack(btn);
    }

    _menu = Menu::createWithArray(items);
    _menu->setPosition(Vec2::ZERO);
    _storePanel->addChild(_menu, 1);
}

void StoreWindow::hide()
{
    this->setVisible(false);
    if (_grayMask) _grayMask->setVisible(false);
    if (_storePanel) _storePanel->setVisible(false);
    this->setTouchEnabled(false);
}

void StoreWindow::onCloseButtonClicked(Ref* sender)
{
    this->hide();
    // 不触发放置
}

// 当前数量（以TownHall为例，其他建筑后续加全局计数）
int StoreWindow::getCurrentCount(BuildingType type)
{
    auto rm = ResourceManager::getInstance();

    if (type == BuildingType::TOWN_HALL)
        return rm->getBuildingCount(StoreWindow::BuildingType::TOWN_HALL);

    if (type == BuildingType::GOLD_MINE) 
        return rm->getBuildingCount(StoreWindow::BuildingType::GOLD_MINE);

    if (type == BuildingType::ELIXIR_COLLECTOR)
        return rm->getBuildingCount(StoreWindow::BuildingType::ELIXIR_COLLECTOR);

    if (type == BuildingType::GOLD_STORAGE) 
        return rm->getBuildingCount(StoreWindow::BuildingType::GOLD_STORAGE);

    if (type == BuildingType::ELIXIR_STORAGE) 
        return rm->getBuildingCount(StoreWindow::BuildingType::ELIXIR_STORAGE);

    if (type == BuildingType::MILITARY_CAMP)
        return rm->getBuildingCount(StoreWindow::BuildingType::MILITARY_CAMP);

    if (type == BuildingType::ARCHER_TOWER)
        return rm->getBuildingCount(StoreWindow::BuildingType::ARCHER_TOWER);

    if (type == BuildingType::CANNON) 
        return rm->getBuildingCount(StoreWindow::BuildingType::CANNON);

    if (type == BuildingType::WORKER_HOME)
        return rm->getBuildingCount(StoreWindow::BuildingType::WORKER_HOME);

	return 0;  // 默认0
}

// 最大数量（从BuildingData拿，以TownHall为例）
int StoreWindow::getMaxCount(BuildingType type, int thLevel)
{
    if (type == BuildingType::TOWN_HALL)
    {
        return TownHallBuildingData.maxBuildCount[thLevel - 1];  // 假设等级从1开始
    }

    else if (type == BuildingType::GOLD_MINE)
    {
        return GoldMineBuildingData.maxBuildCount[thLevel - 1];
    }

    else if (type == BuildingType::ELIXIR_COLLECTOR)
    {
        return ElixirCollectorBuildingData.maxBuildCount[thLevel - 1];
    }

    else if (type == BuildingType::GOLD_STORAGE)
    {
        return GoldStorageBuildingData.maxBuildCount[thLevel - 1];
    }

    else if (type == BuildingType::ELIXIR_STORAGE)
    {
        return ElixirStorageBuildingData.maxBuildCount[thLevel - 1];
    }

    else if (type == BuildingType::MILITARY_CAMP)
    {
        return MilitaryBuildingBuildingData.maxBuildCount[thLevel - 1];
    }

    else if (type == BuildingType::ARCHER_TOWER)
    {
        return ArcherTowerBuildingData.maxBuildCount[thLevel - 1];
    }

    else if (type == BuildingType::CANNON)
    {
        return CanonBuildingData.maxBuildCount[thLevel - 1];
    }

    else if (type == BuildingType::WORKER_HOME)
    {
        return WorkerHomeBuildingData.maxBuildCount[thLevel - 1];
    }
    
    return 0;  // 其他建筑默认5个
}