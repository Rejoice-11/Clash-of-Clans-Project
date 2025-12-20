// Classes/UI/StoreWindow.cpp
#include "StoreWindow.h"
#include "platform/CCFileUtils.h"
#include "base/CCDirector.h"
#include "2d/CCLabel.h"

bool StoreWindow::init()
{
    if (!Layer::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 半透明遮罩
    _backgroundMask = LayerColor::create(Color4B(0, 0, 0, 180), visibleSize.width, visibleSize.height);
    _backgroundMask->setPosition(origin);
    _backgroundMask->setVisible(false);
    this->addChild(_backgroundMask, 20);

    // 商店面板背景
    _panel = Sprite::create("basic_market_bar.png");
    if (!_panel) return false;

    _panel->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    _panel->setVisible(false);
    this->addChild(_panel, 30);

    // 关闭按钮
    auto closeBtn = MenuItemImage::create(
        "out_of_now.png", "out_of_now.png",
        CC_CALLBACK_1(StoreWindow::onCloseButtonClicked, this)
    );
    closeBtn->setPosition(Vec2(_panel->getContentSize().width - 30, _panel->getContentSize().height - 30));
    auto menu = Menu::create(closeBtn, nullptr);
    menu->setPosition(Vec2::ZERO);
    _panel->addChild(menu, 1);

    // 可建造建筑列表：简单示例（真实项目可从 ConfigManager 读取）
    _availableBuildings = { 0, 1, 2 };

    // 创建建筑卡片（水平排列）
    /*float startX = 100;
    for (size_t i = 0; i < _availableBuildings.size(); ++i) {
        int type = _availableBuildings[i];
        const BuildingData& data = ConfigManager::getInstance()->getBuildingData(type);

        bool canAfford = ResourceManager::getInstance()->canAfford(data.cost_gold, data.cost_elixir);

        std::string iconNormal = canAfford ?
            StringUtils::format("building_icon_%d.png", type) :
            "building_icon_locked.png";

        auto card = MenuItemImage::create(
            iconNormal.c_str(), iconNormal.c_str(),
            CC_CALLBACK_1(StoreWindow::onBuildingCardClicked, this)
        );
        card->setTag(type); // 用 tag 存建筑类型
        card->setPosition(Vec2(startX + i * 120, _panel->getContentSize().height - 150));

        auto cardMenu = Menu::create(card, nullptr);
        cardMenu->setPosition(Vec2::ZERO);
        _panel->addChild(cardMenu, 2);

        // 可选：显示名字或价格
        std::string labelStr = StringUtils::format("%d G / %d E", data.cost_gold, data.cost_elixir);
        auto lbl = Label::createWithSystemFont(labelStr, "Arial", 18);
        lbl->setPosition(Vec2(card->getPositionX(), card->getPositionY() - 40));
        _panel->addChild(lbl, 2);
    }*/

    return true;
}

void StoreWindow::show()
{
    _backgroundMask->setVisible(true);
    _panel->setVisible(true);
}

void StoreWindow::hide()
{
    _backgroundMask->setVisible(false);
    _panel->setVisible(false);
    this->removeFromParent();
}

void StoreWindow::onBuildingCardClicked(Ref* sender)
{
    auto item = static_cast<MenuItem*>(sender);
    int buildingType = item->getTag();

    // 使用堆分配的 Value 传递事件数据，接收方负责 delete
    Value* data = new Value(buildingType);
    auto event = EventCustom("building_selected");
    event.setUserData(data);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

    hide();
}

void StoreWindow::onCloseButtonClicked(Ref* sender)
{
    hide();
}