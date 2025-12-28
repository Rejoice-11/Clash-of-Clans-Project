// MilitaryArrange.cpp
#include "MilitaryArrange.h"
#include "Classes/System/ArmyManager.h"

USING_NS_CC;

// 无参 create（和 StoreWindow 完全一致！）
MilitaryArrange* MilitaryArrange::create() {
    MilitaryArrange* layer = new (std::nothrow) MilitaryArrange();
    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

// 无参 init（关键！）
bool MilitaryArrange::init() {
    if (!Layer::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 背景
    _background = Sprite::create("army_arrangement_bg.png");
    if (!_background) {
		return false;
    }
    _background->setPosition(visibleSize / 2);
    this->addChild(_background);

    // 返回按钮
    auto closeBtn = MenuItemImage::create(
        "out_of_now.png", "out_of_now.png",
        CC_CALLBACK_1(MilitaryArrange::onCloseButtonClicked, this)
    );
    closeBtn->setPosition(Vec2(visibleSize.width - 40, visibleSize.height - 40));
    auto menu = Menu::create(closeBtn, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 10);

    // 总兵力显示
    int totalCapacity = ArmyManager::getInstance()->getTotalCapacity();
    _capacityLabel = Label::createWithSystemFont(
        "Total: " + std::to_string(totalCapacity), "Arial", 24
    );
    _capacityLabel->setTextColor(Color4B::WHITE);
    _capacityLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 80));
    this->addChild(_capacityLabel);

    // 已配置军队条
    _troopBar = Node::create();
    _troopBar->setPosition(Vec2(50, visibleSize.height - 150));
    this->addChild(_troopBar);

    // 兵种按钮
    std::vector<std::tuple<UnitType, const char*, int>> troopInfo = {
        {UnitType::MELEE, "berserker_affordable.png", 1},
        {UnitType::RANGED, "archer_affordable.png", 1},
        {UnitType::TANK, "tank_affordable.png", 5},
        {UnitType::WALL_BREAKER, "boomer_affordable.png", 2}
    };

    float startX = 100;
    float startY = 200;
    float spacing = 120;

    for (size_t i = 0; i < troopInfo.size(); ++i) {
        auto [type, iconPath, cost] = troopInfo[i];

        auto normalIcon = Sprite::create(iconPath);
        auto disabledIcon = Sprite::create(iconPath);
        disabledIcon->setColor(Color3B::GRAY);

        auto button = MenuItemSprite::create(
            normalIcon, normalIcon, disabledIcon,
            [this, type, cost](Ref* sender) {
                for (auto& btn : _troopButtons) {
                    if (btn.type == type) {
                        onTroopButtonClicked(&btn);
                        break;
                    }
                }
            }
        );
        button->setPosition(Vec2(startX + static_cast<float>(i) * spacing, startY));
        _troopButtons.push_back({ button, type, cost });
    }

    auto troopMenu = Menu::create();
    for (auto& btn : _troopButtons)
    {
        troopMenu->addChild(btn.button);
    }
    troopMenu->setPosition(Vec2::ZERO);
    this->addChild(troopMenu, 5);

    refreshTroopBar();
    updateButtonStates();

    return true;
}

// 关闭处理（触发回调）
void MilitaryArrange::onCloseButtonClicked(Ref* sender) {
    if (_closeCallback) {
        _closeCallback(); // 通知 VillageScene 清理指针
    }
    this->removeFromParent();
}

void MilitaryArrange::setCloseCallback(const std::function<void()>& callback) 
{
    _closeCallback = callback;
}

void MilitaryArrange::refreshTroopBar()
{
    // 清空旧显示
    for (auto& pair : _troopDisplays)
    {
        pair.first->removeFromParent();
        pair.second->removeFromParent();
    }
    _troopDisplays.clear();

    // 获取当前配置
    auto& pool = ArmyManager::getInstance()->getArmyPool(); // 注意：需将 _armyPool 设为 public 或提供 getter
    float x = 0;
    for (const auto& [type, count] : pool) {
        if (count <= 0) continue;

        // 加载图标
        std::string iconPath;
        switch (type) {
        case UnitType::MELEE: iconPath = "berserker_affordable.png"; break;
        case UnitType::RANGED: iconPath = "archer_affordable.png"; break;
        case UnitType::TANK: iconPath = "tank_affordable.png"; break;
        case UnitType::WALL_BREAKER: iconPath = "boomer_affordable.png"; break;
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

void MilitaryArrange::onTroopButtonClicked(TroopButton* btn)
{
    // 计算总已用兵力
    int used = 0;
    auto& pool = ArmyManager::getInstance()->getArmyPool();

    for (const auto& [type, count] : pool) 
    {
        // 假设每个兵种有固定 cost（需统一管理）
        int cost = 1;
        if (type == UnitType::TANK) 
            cost = 5;

        else if (type == UnitType::WALL_BREAKER) 
            cost = 2;

        else 
            cost = 1;

        used += count * cost;
    }

    // 获取总容量
    int totalCapacity = 0;
    totalCapacity = ArmyManager::getInstance()->getTotalCapacity();

    // 检查是否可添加
    if (used + btn->cost <= totalCapacity)
    {
        auto& pool = ArmyManager::getInstance()->getArmyPool(); // ← 获取非 const 引用
        auto it = pool.find(btn->type);

        if (it == pool.end()) 
        {
            // 该兵种不存在，新增
            pool[btn->type] = 1; // 现在可以了，因为 pool 是非 const
        }

        else 
        {
            // 已存在，+1
            it->second += 1;
        }
        refreshTroopBar();
        updateButtonStates();
    }
}

void MilitaryArrange::updateButtonStates()
{
    // 计算已用兵力
    int used = 0;
    auto& pool = ArmyManager::getInstance()->getArmyPool();
    for (const auto& [type, count] : pool) 
    {
        int cost = 1;

        if (type == UnitType::TANK)
            cost = 5;

        else if (type == UnitType::WALL_BREAKER)
            cost = 2;

        else
            cost = 1;

        used += count * cost;
    }

    int totalCapacity = 0;
    totalCapacity = ArmyManager::getInstance()->getTotalCapacity();

    // 更新按钮状态
    for (auto& btn : _troopButtons) 
    {
        bool canAdd = (used + btn.cost <= totalCapacity);
        btn.button->setEnabled(canAdd);
    }
}
