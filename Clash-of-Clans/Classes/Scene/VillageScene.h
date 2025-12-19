//村庄编辑（主场景）
// scene/VillageScene.h
#pragma once

#include "cocos2d.h"
#include "Classes/Core/GameDirector.h"
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

    // 临时村庄背景（你以后换成正式的）
    auto bg = Sprite::create("village_background.png");
    if (bg) {
        bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        this->addChild(bg, -1);
    }

    // 临时提示文字（证明进来了）
    auto label = Label::createWithTTF("Welcome to Village!\nTaffy的部落启动成功咯~", "fonts/Marker Felt.ttf", 40);
    label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(label, 10);

    return true;
}

void VillageScene::update(float dt)
{
    // 这里处理资源生产、建造倒计时等逻辑
}