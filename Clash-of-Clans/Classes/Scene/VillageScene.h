//村庄编辑（主场景）
// scene/VillageScene.h
#pragma once

#include "cocos2d.h"
#include "Classes/Core/GameDirector.h"
#include "Classes/Scene/BattleScene.h"        // 以后换成敌方村庄或直接BattleScene
USING_NS_CC;

class VillageScene : public Scene
{
public:
    static Scene* createScene();
    virtual bool init() override;
    virtual void update(float dt) override;  // 资源生产、建造倒计时用

    // 按钮回调
    void onAttackButtonClicked(Ref* sender);   // 找敌人打仗
    void onShopButtonClicked(Ref* sender);     // 打开商店
    void onBuilderButtonClicked(Ref* sender);  // 查看工人

    CREATE_FUNC(VillageScene);

private:
    Layer* _backgroundLayer;   // 草地背景 + 装饰
    Layer* _buildingLayer;     // 所有建筑放在这里
    Layer* _uiLayer;           // HUD、按钮等
    LayerColor* _grayMask = nullptr;   // 变灰+吞触控遮罩
    Node* _attackPanel = nullptr;      // 攻击弹窗
    Node* _marketPanel = nullptr;      // 商店全屏
    void onMarketButtonClicked(Ref* sender);
    void closeAttackPanel(Ref* sender);

    // 你需要准备的png
    // "village_background.png"     // 村庄大底图（草地+路径）
    // "button_attack_normal.png"   // 攻击按钮
    // "button_attack_pressed.png"
    // "button_shop_normal.png"
    // "button_builder_normal.png"
    // "hud_top_bar.png"            // 顶部资源栏背景（后面HUDLayer用）
};

// scene/VillageScene.cpp

Scene* VillageScene::createScene()
{
    auto scene = Scene::create();
    auto layer = VillageScene::create();
    if (layer) {
        scene->addChild(layer);
    }
    return scene;
}

bool VillageScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 1. 村庄背景
    auto bg = Sprite::create("village_background.png");
    if (bg) {
        auto centerPos = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
        bg->setPosition(centerPos);
        this->addChild(bg, -1);
    }

    // 2. 左下角 攻击按钮
    auto attackBtn = MenuItemImage::create(
        "attack_button.png", "attack_button.png",
        CC_CALLBACK_1(VillageScene::onAttackButtonClicked, this));
    attackBtn->setPosition(Vec2(100 + origin.x, 100 + origin.y));  // 左下角

    // 3. 右下角 商店按钮
    auto marketBtn = MenuItemImage::create(
        "market_button.png", "market_button.png",
        CC_CALLBACK_1(VillageScene::onMarketButtonClicked, this));
    marketBtn->setPosition(Vec2(visibleSize.width - 100 + origin.x, 100 + origin.y));  // 右下角

    auto menu = Menu::create(attackBtn, marketBtn, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 10);

    // 预先创建遮罩层（变灰+吞触控）
    _grayMask = LayerColor::create(Color4B(0, 0, 0, 180), visibleSize.width, visibleSize.height);
    _grayMask->setPosition(origin);
    _grayMask->setVisible(false);
    this->addChild(_grayMask, 20);

    return true;
}

// 攻击按钮点击 → 弹出左侧攻击面板
void VillageScene::onAttackButtonClicked(Ref* sender)
{
    if (_attackPanel || _marketPanel) return;  // 防止重复打开

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 变灰遮罩
    _grayMask->setVisible(true);

    // 攻击弹窗背景（左侧）
    auto panel = Sprite::create("attack_out_when_point_the_attack_button_in_village_scene.png");
    if (panel) {
        panel->setAnchorPoint(Vec2(0, 0.5f));
        panel->setPosition(Vec2(origin.x, visibleSize.height / 2 + origin.y));
        this->addChild(panel, 30);
        _attackPanel = panel;

        // 去战斗按钮（中间）
        auto fightBtn = MenuItemImage::create(
            "attack_button_to_reverse_to_battle_scene.png",
            "attack_button_to_reverse_to_battle_scene.png",
            [this](Ref*) {
                this->closeAttackPanel(nullptr);
                GameDirector::getInstance()->replaceScene(BattleScene::createScene());
            });

        // 关闭按钮（右上角）
        auto closeBtn = MenuItemImage::create(
            "out_of_now.png", "out_of_now.png",
            CC_CALLBACK_1(VillageScene::closeAttackPanel, this));

        closeBtn->setPosition(Vec2(panel->getContentSize().width - 40,
            panel->getContentSize().height - 40));

        fightBtn->setPosition(panel->getContentSize() / 2);

        auto menu = Menu::create(fightBtn, closeBtn, nullptr);
        menu->setPosition(Vec2::ZERO);
        panel->addChild(menu, 1);
    }
}

// 关闭攻击面板
void VillageScene::closeAttackPanel(Ref*)
{
    if (_attackPanel) {
        _attackPanel->removeFromParent();
        _attackPanel = nullptr;
    }
    _grayMask->setVisible(false);
}

// 商店按钮点击 → 全屏商店
void VillageScene::onMarketButtonClicked(Ref* sender)
{
    if (_attackPanel || _marketPanel) return;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _grayMask->setVisible(true);

    auto market = Sprite::create("basic_market_bar.png");
    if (market) {
        auto centerPos = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
        market->setPosition(centerPos);
        this->addChild(market, 30);
        _marketPanel = market;

        // 右上角关闭按钮
        auto closeBtn = MenuItemImage::create(
            "out_of_now.png", "out_of_now.png",
            [this](Ref*) {
                if (_marketPanel) {
                    _marketPanel->removeFromParent();
                    _marketPanel = nullptr;
                }
                _grayMask->setVisible(false);
            });

        closeBtn->setPosition(Vec2(market->getContentSize().width - 60,
            market->getContentSize().height - 60));

        auto menu = Menu::create(closeBtn, nullptr);
        menu->setPosition(Vec2::ZERO);
        market->addChild(menu, 1);
    }
}

void VillageScene::update(float dt)
{
    //waiting for implementation: 资源生产、建造倒计时等逻辑
}