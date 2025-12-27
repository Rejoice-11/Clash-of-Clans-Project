// BuildingPanel.cpp
#include "BuildingPanel.h"
#include "Classes/System/ResourceManager.h"
#include "Classes/Entity/Building/ResourceBuilding.h"
#include "Classes/Entity/Building/StorageBuilding.h"
#include "Classes/Entity/Building/DefenseBuilding.h"
#include "Classes/Entity/Building/TownHall.h"

BuildingPanel* BuildingPanel::create(Building* building, const std::function<void()>& onClose) 
{
    auto panel = new (std::nothrow) BuildingPanel();
    if (panel && panel->init(building, onClose)) {
        panel->autorelease();
        return panel;
    }
    CC_SAFE_DELETE(panel);
    return nullptr;
}

bool BuildingPanel::init(Building* building, const std::function<void()>& onClose) {
    if (!Layer::init()) return false;

    _building = building;
    _onCloseCallback = onClose;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 背景
    auto bg = Sprite::create("building_panel_background.png");
    bg->setPosition(Vec2(visibleSize / 2) + origin);
    this->addChild(bg);

    // 建筑图片（左）
    std::string spriteName = "";
    switch (_building->getBuildingType()) 
    {
        case BuildingType::TOWN_HALL:
            spriteName = "town_hall_lv" + std::to_string(_building->getCurrentLevel()) + "_sub.png";
            break;

        case BuildingType::GOLD_MINE:
            spriteName = "gold_mine_lv" + std::to_string(_building->getCurrentLevel()) + "_sub.png"; 
            break;

        case BuildingType::ELIXIR_COLLECTOR:
            spriteName = "elixir_collector_lv" + std::to_string(_building->getCurrentLevel()) + "_sub.png"; 
            break;

        case BuildingType::GOLD_STORAGE:
            spriteName = "gold_storage_lv" + std::to_string(_building->getCurrentLevel()) + "_sub.png";
            break;

        case BuildingType::ELIXIR_STORAGE:
            spriteName = "elixir_storage_lv" + std::to_string(_building->getCurrentLevel()) + "_sub.png";
            break;

        case BuildingType::ARCHER_TOWER:
            spriteName = "archer_tower_lv" + std::to_string(_building->getCurrentLevel()) + "_sub.png";
            break;

        case BuildingType::CANNON:
            spriteName = "canon_lv" + std::to_string(_building->getCurrentLevel()) + "_sub.png";
            break;

        default: spriteName = "worker_home_lv0.png";
    }

    _buildingImage = Sprite::create(spriteName);
    if (_buildingImage) 
    {
        _buildingImage->setPosition(Vec2(180, bg->getContentSize().height / 2 - 30));
        bg->addChild(_buildingImage);
    }

    // 建筑类型+等级（上）
    std::string typeName = "";
    switch (_building->getBuildingType()) 
    {
        case BuildingType::TOWN_HALL: typeName = u8"大本营"; 
            break;

        case BuildingType::GOLD_MINE: typeName = u8"金矿";
            break;

        case BuildingType::ELIXIR_COLLECTOR: typeName = u8"圣水收集器";
            break;

        case BuildingType::GOLD_STORAGE: typeName = u8"储金罐";
            break;

        case BuildingType::ELIXIR_STORAGE: typeName = u8"圣水瓶";
            break;

        case BuildingType::ARCHER_TOWER: typeName = u8"箭塔";
            break;

        case BuildingType::CANNON: typeName = u8"加农炮";
            break;

        default: typeName = u8"建筑者小屋";
    }

    _levelLabel = Label::createWithSystemFont(
        typeName + u8" 等级: " + std::to_string(_building->getCurrentLevel()),
        "arial", 24
    );
	_levelLabel->setTextColor(Color4B::WHITE);
    _levelLabel->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height - 50));
    bg->addChild(_levelLabel);

    // 属性列表（右）
    float startY = bg->getContentSize().height - 110;
    float spacing = 60;
    auto addStat = [&](const std::string& icon, const std::string& value, int index)
    {
        auto iconSprite = Sprite::create(icon);
        if (iconSprite) 
        {
            iconSprite->setPosition(Vec2(475, startY - index * spacing));
            bg->addChild(iconSprite);

            auto label = Label::createWithSystemFont(value, "arial", 20);
            label->setTextColor(Color4B::BLACK);
            label->enableOutline(Color4B::WHITE, 2);
            label->setPosition(Vec2(500, startY - 10 - index * spacing));
            bg->addChild(label);
            _statLabels.push_back(label);
        }
    };

    // 根据建筑类型添加属性
    int statIndex = 0;
    if (auto* rb = dynamic_cast<ResourceBuilding*>(_building)) 
    {
        if (rb->getResourceType() == ResourceBuilding::ResourceType::GOLD)
        {
            addStat("gold_produce_per_hour.png", std::to_string(GoldMineBuildingData.productionPerHour[rb->getCurrentLevel() - 1]), statIndex++);
            addStat("hit_point.png", std::to_string(GoldMineBuildingData.hitPoints[rb->getCurrentLevel() - 1]), statIndex++);
        }

        else 
        {
            addStat("elixir_produce_per_hour.png", std::to_string(ElixirCollectorBuildingData.productionPerHour[rb->getCurrentLevel() - 1]), statIndex++);
            addStat("hit_point.png", std::to_string(ElixirCollectorBuildingData.hitPoints[rb->getCurrentLevel() - 1]), statIndex++);
        }
  
    }

    else if (auto* sb = dynamic_cast<StorageBuilding*>(_building))
    {
        if (sb->getStorageType() == StorageBuilding::StorageType::GOLD_STORAGE)
        {
            addStat("gold_storage.png", std::to_string(sb->getCapacity()), statIndex++);
            addStat("hit_point.png", std::to_string(GoldStorageBuildingData.hitPoints[sb->getCurrentLevel() - 1]), statIndex++);
        }

        else 
        {
            addStat("elixir_storage.png", std::to_string(sb->getCapacity()), statIndex++);
            addStat("hit_point.png", std::to_string(ElixirStorageBuildingData.hitPoints[sb->getCurrentLevel() - 1]), statIndex++);
        }

    }

    else if (auto* db = dynamic_cast<DefenseBuilding*>(_building)) 
    {
        if (db->getDefenseType() == DefenseBuilding::DefenseType::CANON)
        {
            addStat("attack_range.png", std::to_string(CanonBuildingData.attackRange[db->getCurrentLevel() - 1]), statIndex++);
            addStat("damage_per_second.png", std::to_string(CanonBuildingData.damagePerSecond[db->getCurrentLevel() - 1]), statIndex++);
            addStat("hit_point.png", std::to_string(CanonBuildingData.hitPoints[db->getCurrentLevel() - 1]), statIndex++);
        }

        else
        {
            addStat("attack_range.png", std::to_string(ArcherTowerBuildingData.attackRange[db->getCurrentLevel() - 1]), statIndex++);
            addStat("damage_per_second.png", std::to_string(ArcherTowerBuildingData.damagePerSecond[db->getCurrentLevel() - 1]), statIndex++);
            addStat("hit_point.png", std::to_string(ArcherTowerBuildingData.hitPoints[db->getCurrentLevel() - 1]), statIndex++);
        }
    }

    else if (dynamic_cast<TownHall*>(_building)) 
    {
        addStat("hit_point.png", std::to_string(ArcherTowerBuildingData.hitPoints[_building->getCurrentLevel() - 1]), statIndex++);
    }

    // 升级按钮（右下）
    int upgradeCost = 0;
    bool isGold = true;
    std::string costIcon = "upgrade_by_coin.png";

    if (dynamic_cast<TownHall*>(_building)) 
    {
        upgradeCost = TownHallBuildingData.goldCost[_building->getCurrentLevel() - 1];
    }

    else if (dynamic_cast<ResourceBuilding*>(_building)) 
    {
        auto rb = static_cast<ResourceBuilding*>(_building);
        if (rb->getResourceType() == ResourceBuilding::ResourceType::GOLD) 
        {
            upgradeCost = GoldMineBuildingData.elixirCost[_building->getCurrentLevel() - 1];
            isGold = false;
            costIcon = "upgrade_by_elixir.png";
        }

        else 
        {
            upgradeCost = ElixirCollectorBuildingData.goldCost[_building->getCurrentLevel() - 1];
        }
    }

    else if (dynamic_cast<StorageBuilding*>(_building))
    {
        auto sb = static_cast<StorageBuilding*>(_building);
        if (sb->getStorageType() == StorageBuilding::StorageType::GOLD_STORAGE)
        {
            upgradeCost = GoldStorageBuildingData.elixirCost[_building->getCurrentLevel() - 1];
            isGold = false;
            costIcon = "upgrade_by_elixir.png";
        }

        else
        {
            upgradeCost = ElixirStorageBuildingData.goldCost[_building->getCurrentLevel() - 1];
        }
    }

    else if (dynamic_cast<DefenseBuilding*>(_building)) 
    {
        auto db = static_cast<DefenseBuilding*>(_building);
        if (db->getDefenseType() == DefenseBuilding::DefenseType::CANON)
        {
            upgradeCost = CanonBuildingData.elixirCost[_building->getCurrentLevel() - 1];
            isGold = false;
            costIcon = "upgrade_by_elixir.png";
        }

        else
        {
            upgradeCost = ArcherTowerBuildingData.goldCost[_building->getCurrentLevel() - 1];
        }
	}

    else
    {
        // 默认处理
        upgradeCost = 100; // 示例默认值unfinished
	}

    auto upgradeBtn = MenuItemImage::create(costIcon, costIcon, CC_CALLBACK_1(BuildingPanel::onUpdateButtonClicked, this));
    upgradeBtn->setPosition(Vec2(480, 50));

    _costLabel = Label::createWithSystemFont(std::to_string(upgradeCost), "arial", 24);
    _costLabel->setTextColor(isGold ? Color4B(255, 215, 0, 255) : Color4B(186, 85, 211, 255));
    _costLabel->enableOutline(Color4B::BLACK, 2);
    _costLabel->setPosition(Vec2(480, 60));
    bg->addChild(_costLabel);

    // 关闭按钮（右上）
    auto closeBtn = MenuItemImage::create("out_of_now.png", "out_of_now.png", CC_CALLBACK_1(BuildingPanel::onCloseButtonClicked, this));
    closeBtn->setPosition(Vec2(bg->getContentSize().width - 40, bg->getContentSize().height - 40));
    auto menu = Menu::create(upgradeBtn,closeBtn, nullptr);
    menu->setPosition(Vec2::ZERO);
    bg->addChild(menu);

    return true;
}

void BuildingPanel::onUpdateButtonClicked(Ref* sender) 
{
    if (_building->getCurrentLevel() >= MAX_LEVELS) return;

    // 获取升级成本
    int cost = 0;
    bool isGold = true;
    if (dynamic_cast<TownHall*>(_building)) 
    {
        cost = TownHallBuildingData.goldCost[_building->getCurrentLevel() - 1];
    }
    else if (dynamic_cast<ResourceBuilding*>(_building)) 
    {
        auto rb = static_cast<ResourceBuilding*>(_building);
        if (rb->getResourceType() == ResourceBuilding::ResourceType::GOLD) 
        {
            cost = GoldMineBuildingData.elixirCost[_building->getCurrentLevel() - 1];
            isGold = false;
        }

        else 
        {
            cost = ElixirCollectorBuildingData.goldCost[_building->getCurrentLevel() - 1];
        }
    }
    else if (dynamic_cast<StorageBuilding*>(_building))
    {
        auto sb = static_cast<StorageBuilding*>(_building);
        if (sb->getStorageType() == StorageBuilding::StorageType::GOLD_STORAGE)
        {
            cost = GoldStorageBuildingData.elixirCost[_building->getCurrentLevel() - 1];
            isGold = false;
        }
        else
        {
            cost = ElixirStorageBuildingData.goldCost[_building->getCurrentLevel() - 1];
        }
    }
    else if (dynamic_cast<DefenseBuilding*>(_building)) 
    {
        auto db = static_cast<DefenseBuilding*>(_building);
        if (db->getDefenseType() == DefenseBuilding::DefenseType::CANON)
        {
            cost = CanonBuildingData.elixirCost[_building->getCurrentLevel() - 1];
            isGold = false;
        }
        else
        {
            cost = ArcherTowerBuildingData.goldCost[_building->getCurrentLevel() - 1];
        }
    }
    else 
    {
        // 默认处理
        cost = 100; // 示例默认值unfinished
	}

    // 扣除资源
    auto rm = ResourceManager::getInstance();
    bool canAfford = isGold ? rm->spendGold(cost) : rm->spendElixir(cost);

    if (!canAfford) 
        return;

    // 升级建筑
    _building->upgrade();
    SimpleAudioEngine::getInstance()->playEffect("audio/level_up.mp3");
    // 刷新面板
    refreshPanel();
}

void BuildingPanel::refreshPanel()
{
    // 更新等级显示
    std::string typeName = "";
    switch (_building->getBuildingType())
    {
        case BuildingType::TOWN_HALL: typeName = u8"大本营";
            break;

        case BuildingType::GOLD_MINE: typeName = u8"金矿";
            break;

		case BuildingType::ELIXIR_COLLECTOR: typeName = u8"圣水收集器";
			break;

		case BuildingType::GOLD_STORAGE: typeName = u8"储金罐";
			break;

		case BuildingType::ELIXIR_STORAGE: typeName = u8"圣水瓶";
			break;

		case BuildingType::ARCHER_TOWER: typeName = u8"箭塔";
			break;

		case BuildingType::CANNON: typeName = u8"加农炮";
			break;

            // ... 其他类型 ...unfinished
        default: typeName = "Building";
    }
    _levelLabel->setString(typeName + u8" 等级: " + std::to_string(_building->getCurrentLevel()));

    // 更新建筑图片
    std::string spriteName = "";
    switch (_building->getBuildingType()) 
    {
        case BuildingType::TOWN_HALL:
            spriteName = "town_hall_lv" + std::to_string(_building->getCurrentLevel()) + ".png";
            break;

        case BuildingType::GOLD_MINE:
			spriteName = "gold_mine_lv" + std::to_string(_building->getCurrentLevel()) + ".png";
            break;

		case BuildingType::ELIXIR_COLLECTOR:
			spriteName = "elixir_collector_lv" + std::to_string(_building->getCurrentLevel()) + ".png"; 
            break;

		case BuildingType::GOLD_STORAGE:
			spriteName = "gold_storage_lv" + std::to_string(_building->getCurrentLevel()) + ".png";
            break;

		case BuildingType::ELIXIR_STORAGE:
			spriteName = "elixir_storage_lv" + std::to_string(_building->getCurrentLevel()) + ".png";
            break;

		case BuildingType::ARCHER_TOWER:
			spriteName = "archer_tower_lv" + std::to_string(_building->getCurrentLevel()) + ".png";
            break;

		case BuildingType::CANNON:
            spriteName = "canon_lv" + std::to_string(_building->getCurrentLevel()) + ".png";
            break;

		default: spriteName = "";
    }

    if (!spriteName.empty()) 
    {
        _buildingImage->setTexture(spriteName);
    }

    // 重新计算属性值（简化：直接重建面板）
    // 实际项目中可优化为只更新数值
    this->removeAllChildren();
    init(_building, _onCloseCallback); // 重新初始化
}

void BuildingPanel::onCloseButtonClicked(Ref* sender)
{
    if (_onCloseCallback)
        _onCloseCallback();

    this->removeFromParent();
}