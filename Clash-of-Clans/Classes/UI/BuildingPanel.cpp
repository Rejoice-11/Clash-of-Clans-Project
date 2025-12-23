#include "BuildingPanel.h"
#include "Classes/System/ResourceManager.h"

BuildingPanel* BuildingPanel::create(Building* building, const std::function<void()>& onClose) {
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

    auto bg = Sprite::create("building_panel_background.png");
    bg->setPosition(visibleSize / 2 + cocos2d::Size(origin));
    this->addChild(bg);

    // 显示建筑信息
    _levelLabel = Label::createWithSystemFont(
        "Level: " + std::to_string(_building->getCurrentLevel()),
        "Arial", 24
    );
    _levelLabel->setPosition(bg->getContentSize() / 2);
    bg->addChild(_levelLabel);

    // 升级按钮
    auto updateBtn = MenuItemImage::create(
        "update_this_building.png",
        "update_this_building.png",
        CC_CALLBACK_1(BuildingPanel::onUpdateButtonClicked, this)
    );
    updateBtn->setPosition(Vec2(bg->getContentSize().width - 100, 100));
    auto menu = Menu::create(updateBtn, nullptr);
    menu->setPosition(Vec2::ZERO);
    bg->addChild(menu);

    // 关闭按钮
    auto closeBtn = MenuItemImage::create(
        "out_of_now.png",
        "out_of_now.png",
        CC_CALLBACK_1(BuildingPanel::onCloseButtonClicked, this)
    );
    closeBtn->setPosition(Vec2(bg->getContentSize().width - 40, bg->getContentSize().height - 40));
    bg->addChild(closeBtn);

    return true;
}

void BuildingPanel::onUpdateButtonClicked(Ref* sender) {
    if (_building->getCurrentLevel() >= MAX_LEVELS) {
        return; // 已达最高等级
    }

    // 检查资源
    int cost = 0;
    bool isGold = true;
    // 根据建筑类型获取升级费用（示例）
    if (dynamic_cast<TownHall*>(_building)) {
        cost = TownHallBuildingData.goldCost[_building->getCurrentLevel()];
    }
    else if (dynamic_cast<ResourceBuilding*>(_building)) {
        auto rb = static_cast<ResourceBuilding*>(_building);
        if (rb->getResourceType() == ResourceBuilding::ResourceType::GOLD) {
            cost = GoldMineBuildingData.elixirCost[_building->getCurrentLevel()];
            isGold = false;
        }
        else {
            cost = ElixirCollectorBuildingData.goldCost[_building->getCurrentLevel()];
        }
    }

    auto rm = ResourceManager::getInstance();
    bool canAfford = isGold ? rm->spendGold(cost) : rm->spendElixir(cost);
    if (!canAfford) return;

    // 升级建筑
    _building->upgrade();
    _levelLabel->setString("Level: " + std::to_string(_building->getCurrentLevel()));

    // 刷新 VillageScene 中的精灵（需要实现）
    // VillageScene::getInstance()->refreshBuildingSprite(_building);
}

void BuildingPanel::onCloseButtonClicked(Ref* sender) {
    if (_onCloseCallback) _onCloseCallback();
    this->removeFromParent();
}