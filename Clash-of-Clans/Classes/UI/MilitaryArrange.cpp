// MilitaryArrang.cpp
/*#include "MilitaryArrange.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

bool MilitaryArrang::init(VillageScene* villageScene)
{
    if (!Layer::init()) 
        return false;

    _villageScene = villageScene;
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 背景
    _background = Sprite::create("army_arrangement_bg.png");
    if (!_background) {
        _background = LayerColor::create(Color4B(30, 30, 50, 220));
        _background->setContentSize(visibleSize);
    }
    _background->setPosition(visibleSize / 2);
    this->addChild(_background);

    // 返回按钮
    auto closeBtn = MenuItemImage::create(
        "out_of_now.png", "out_of_now.png",
        CC_CALLBACK_1(MilitaryArrang::onCloseButtonClicked, this)
    );
    closeBtn->setPosition(Vec2(visibleSize.width - 40, visibleSize.height - 40));
    auto menu = Menu::create(closeBtn, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 10);

    // 总兵力显示
    int totalCapacity = 0;
    if (!_villageScene->_militaryCamps.empty()) {
        totalCapacity = _villageScene->_militaryCamps[0]->getTotalTroopCapacity();
    }
    _capacityLabel = Label::createWithSystemFont(
        "Total: " + std::to_string(totalCapacity), "Arial", 24
    );
    _capacityLabel->setTextColor(Color4B::WHITE);
    _capacityLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 80));
    this->addChild(_capacityLabel);

    // 已配置军队条（横杠）
    _troopBar = Node::create();
    _troopBar->setPosition(Vec2(50, visibleSize.height - 150));
    this->addChild(_troopBar);

    // 兵种按钮（假设4种兵种）
    std::vector<std::tuple<UnitType, const char*, int>> troopInfo = {
        {UnitType::BARBARIAN, "barbarian_icon.png", 1},
        {UnitType::ARCHER, "archer_icon.png", 1},
        {UnitType::GIANT, "giant_icon.png", 5},
        {UnitType::GOBLIN, "goblin_icon.png", 1}
    };

    float startX = 100;
    float startY = 200;
    float spacing = 120;

    for (int i = 0; i < troopInfo.size(); ++i) {
        auto [type, iconPath, cost] = troopInfo[i];
        auto normalIcon = Sprite::create(iconPath);
        auto disabledIcon = Sprite::create(iconPath);
        disabledIcon->setColor(Color3B::GRAY);

        auto button = MenuItemSprite::create(
            normalIcon, normalIcon, disabledIcon,
            [this, type, cost](Ref* sender) {
                // 找到对应的按钮
                for (auto& btn : _troopButtons) {
                    if (btn.type == type) {
                        onTroopButtonClicked(&btn);
                        break;
                    }
                }
            }
        );
        button->setPosition(Vec2(startX + i * spacing, startY));

        _troopButtons.push_back({ button, type, cost });
    }

    auto troopMenu = Menu::create();
    for (auto& btn : _troopButtons) {
        troopMenu->addChild(btn.button);
    }
    troopMenu->setPosition(Vec2::ZERO);
    this->addChild(troopMenu, 5);

    // 初始刷新
    refreshTroopBar();
    updateButtonStates();

    return true;
}

void MilitaryArrang::refreshTroopBar()
{
    // 清空旧显示
    for (auto& pair : _troopDisplays)
    {
        pair.first->removeFromParent();
        pair.second->removeFromParent();
    }
    _troopDisplays.clear();

    // 获取当前配置
    auto& pool = ArmyManager::getInstance()->_armyPool; // 注意：需将 _armyPool 设为 public 或提供 getter
    float x = 0;
    for (const auto& [type, count] : pool) {
        if (count <= 0) continue;

        // 加载图标
        std::string iconPath;
        switch (type) {
        case UnitType::MELEE: iconPath = "barbarian_icon.png"; break;
        case UnitType::RANGED: iconPath = "archer_icon.png"; break;
        case UnitType::TANK: iconPath = "giant_icon.png"; break;
        case UnitType::WALL_BREAKER: iconPath = "goblin_icon.png"; break;
        default: continue;
        }

        auto icon = Sprite::create(iconPath);
        icon->setScale(0.8f);
        icon->setPosition(Vec2(x, 0));
        _troopBar->addChild(icon);

        auto label = Label::createWithSystemFont(std::to_string(count), "Arial", 20);
        label->setTextColor(Color4B::WHITE);
        label->setPosition(Vec2(x, -30));
        _troopBar->addChild(label);

        _troopDisplays.emplace_back(icon, label);
        x += 80; // 间距
    }
}

void MilitaryArrang::onTroopButtonClicked(TroopButton* btn) {
    // 计算总已用兵力
    int used = 0;
    auto& pool = ArmyManager::getInstance()->_armyPool;
    for (const auto& [type, count] : pool) {
        // 假设每个兵种有固定 cost（需统一管理）
        int cost = 1;
        if (type == UnitType::GIANT) cost = 5;
        used += count * cost;
    }

    // 获取总容量
    int totalCapacity = 0;
    if (!_villageScene->_militaryCamps.empty()) {
        totalCapacity = _villageScene->_militaryCamps[0]->getTotalTroopCapacity();
    }

    // 检查是否可添加
    if (used + btn->cost <= totalCapacity) {
        ArmyManager::getInstance()->setUnitCount(
            btn->type,
            pool[btn->type] + 1
        );
        refreshTroopBar();
        updateButtonStates();
    }
}

void MilitaryArrang::updateButtonStates() {
    // 计算已用兵力
    int used = 0;
    auto& pool = ArmyManager::getInstance()->_armyPool;
    for (const auto& [type, count] : pool) {
        int cost = (type == UnitType::GIANT) ? 5 : 1;
        used += count * cost;
    }

    int totalCapacity = 0;
    if (!_villageScene->_militaryCamps.empty()) {
        totalCapacity = _villageScene->_militaryCamps[0]->getTotalTroopCapacity();
    }

    // 更新按钮状态
    for (auto& btn : _troopButtons) {
        bool canAdd = (used + btn.cost <= totalCapacity);
        btn.button->setEnabled(canAdd);
    }
}

void MilitaryArrang::onCloseButtonClicked(Ref* sender) {
    this->removeFromParent();
}*/
