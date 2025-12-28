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
    closeBtn->setPosition(Vec2(visibleSize.width - 220, visibleSize.height - 150));
    auto menu = Menu::create(closeBtn, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 10);

    // 总兵力显示
    int totalCapacity = ArmyManager::getInstance()->getTotalCapacity();
    _capacityLabel = Label::createWithSystemFont(
        u8"最大数量 " + std::to_string(totalCapacity), "Arial", 24
    );
    _capacityLabel->setTextColor(Color4B::WHITE);
    _capacityLabel->setPosition(Vec2(visibleSize.width / 2 - 280, visibleSize.height - 190));
    this->addChild(_capacityLabel);

	int usedCapacity = ArmyManager::getInstance()->getUsedCapacity();
    _usedCapacityLabel = Label::createWithSystemFont(
        u8"已用数量 " + std::to_string(usedCapacity), "Arial", 24
	);
	_usedCapacityLabel->setTextColor(Color4B::WHITE);
	_usedCapacityLabel->setPosition(Vec2(visibleSize.width / 2 - 100, visibleSize.height - 190));
	this->addChild(_usedCapacityLabel);
  

    // 已配置军队条
    _troopBar = Node::create();
    _troopBar->setPosition(Vec2(375, visibleSize.height - 300));
    this->addChild(_troopBar);

    // 兵种按钮
    std::vector<std::tuple<UnitType, const char*, int>> troopInfo = {
        {UnitType::MELEE, "berserker_affordable.png", 1},
        {UnitType::RANGED, "archer_affordable.png", 1},
        {UnitType::TANK, "tank_affordable.png", 5},
        {UnitType::WALL_BREAKER, "boomer_affordable.png", 2}
    };

    float startX = 350;
    float startY = 240;
    float spacing = 175;

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

    // === 新增：计算并更新已用兵力 ===
    int usedCapacity = ArmyManager::getInstance()->getUsedCapacity();
    if (!_usedCapacityLabel) 
    {
        // 首次创建标签
        _usedCapacityLabel = Label::createWithSystemFont(
            u8"已使用兵量:" + std::to_string(usedCapacity), "Arial", 24
               );
        _usedCapacityLabel->setTextColor(Color4B::WHITE);
        _usedCapacityLabel->setPosition(Vec2(
            Director::getInstance()->getVisibleSize().width / 2 + 100,
            Director::getInstance()->getVisibleSize().height - 80
            ));
        this->addChild(_usedCapacityLabel);

    }
    else
    {
        // 更新现有标签
        _usedCapacityLabel->setString(u8"已使用兵量:" + std::to_string(usedCapacity));
    }
    // ==============================

    float x = 0;
    for (const auto& [type, count] : pool)
    {
        if (count <= 0) continue;

        // 加载图标
        // 创建图标（可点击！）
        std::string iconPath = getIconPathForType(type);
        auto icon = Sprite::create(iconPath);
        icon->setScale(0.8f);
        icon->setPosition(Vec2(x, 0));

        UnitType capturedType = type;

        // === 关键：添加点击事件 ===
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [this, capturedType](Touch* touch, Event* event) {
            this->onExistingTroopClicked(capturedType);
            return true;
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, icon);
        // ========================

        _troopBar->addChild(icon);

		std::string labelStr = std::string(u8"已有:") + std::to_string(count);
        auto label = Label::createWithSystemFont(labelStr, "Arial", 20);
        label->setTextColor(Color4B::BLACK);
        label->setPosition(Vec2(x, 80));
        _troopBar->addChild(label);

        _troopDisplays.emplace_back(icon, label);
        x += 175; // 间距
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

    ArmyManager::getInstance()->setUsedCapacity(used);

    int totalCapacity = 0;
    totalCapacity = ArmyManager::getInstance()->getTotalCapacity();

    // 更新按钮状态
    for (auto& btn : _troopButtons) 
    {
        bool canAdd = (used + btn.cost <= totalCapacity);
        btn.button->setEnabled(canAdd);
    }
}

// MilitaryArrange.cpp
std::string MilitaryArrange::getIconPathForType(UnitType type)
{
    switch (type) {
    case UnitType::MELEE: return "berserker_affordable.png";
    case UnitType::RANGED: return "archer_affordable.png";
    case UnitType::TANK: return "tank_affordable.png";
    case UnitType::WALL_BREAKER: return "boomer_affordable.png";
    default: return "default_troop.png";
    }
}

// MilitaryArrange.cpp
void MilitaryArrange::onExistingTroopClicked(UnitType type) 
{
    auto& pool = ArmyManager::getInstance()->getArmyPool();
    auto it = pool.find(type);

    if (it != pool.end() && it->second > 0) 
    {
        // 减少一个单位
        it->second--;

        // 如果数量为0，从map中移除（可选）
        if (it->second == 0) {
            pool.erase(it);
        }

        // 刷新显示
        refreshTroopBar();
        updateButtonStates(); // 重新启用可能被禁用的按钮
    }
}
