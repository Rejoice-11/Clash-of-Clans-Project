//战斗模拟
// scene/BattleScene.h
#pragma once

#include "cocos2d.h"
#include "Classes/Core/GameDirector.h"
USING_NS_CC;

class BattleScene : public Scene
{
public:
    static Scene* createScene();
    virtual bool init() override;
    virtual void update(float dt) override;  // 战斗实时模拟核心！！

    // 按钮回调
    void onSurrenderButtonClicked(Ref* sender);  // 投降
    void onNextWaveButtonClicked(Ref* sender);   // 预留下一波

    CREATE_FUNC(BattleScene);

private:
    Layer* _backgroundLayer;   // 敌方村庄背景
    Layer* _buildingLayer;     // 敌方建筑
    Layer* _unitLayer;         // 玩家部队 + 子弹
    Layer* _effectLayer;       // 爆炸特效等
    Layer* _uiLayer;           // 战斗HUD（剩余部队、时间、星级）

    // 你需要准备的png
    // "battle_background.png"      // 可以和村庄底图共用，或者单独一套
    // "battle_deploy_bar.png"      // 底部部队部署栏背景
    // "button_surrender_normal.png"
    // "button_surrender_pressed.png"
    // "star_empty.png" / "star_full.png"  // 星级显示用
    // "timer_icon.png"             // 战斗倒计时图标
};

Scene* BattleScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BattleScene::create();
    if (layer) {
        scene->addChild(layer);
    }
    return scene;
}

bool BattleScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 唯一一张背景图（全屏铺开）
    auto background = Sprite::create("village_background.png");
    if (background) {
        // 让图片填满屏幕（等比缩放）
        float scaleX = visibleSize.width / background->getContentSize().width;
        float scaleY = visibleSize.height / background->getContentSize().height;
        float scale = MAX(scaleX, scaleY);
        background->setScale(scale);

        background->setPosition(Vec2(visibleSize.width / 2 + origin.x,
            visibleSize.height / 2 + origin.y));
        this->addChild(background, -1);
    }

    // 临时提示文字（证明成功进入战斗场景）
    auto label = Label::createWithTTF("Battle Scene!\nTaffy的屠村时间到咯~",
        "fonts/Marker Felt.ttf", 48);
    if (label) {
        label->setPosition(Vec2(visibleSize.width / 2 + origin.x,
            visibleSize.height / 2 + origin.y));
        label->setColor(Color3B::RED);
        this->addChild(label, 10);
    }

    // 预留update（以后部队移动、战斗计算全靠这个）
    this->scheduleUpdate();

    return true;
}

void BattleScene::update(float dt)
{
    // 暂时空着，以后放战斗逻辑
    // 比如：部队寻路、伤害结算、胜利判定啥的
}