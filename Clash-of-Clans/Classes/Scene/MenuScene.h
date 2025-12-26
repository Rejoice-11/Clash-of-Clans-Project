//开始界面
// scene/MenuScene.h
#pragma once

#include "cocos2d.h"
#include "Classes/Scene/MenuScene.h"
#include "Classes/Scene/VillageScene.h"       // 等会儿自动跳转用
#include "Classes/Core/GameDirector.h"
#include "Classes/Core/ConfigManagerUnit.h"       // 假设你要在这里加载所有JSON
USING_NS_CC;

class MenuScene : public Scene
{
public:
    static Scene* createScene();
    virtual bool init() override;

    // 按钮回调
    void onPlayButtonClicked(Ref* sender);
    void onSettingsButtonClicked(Ref* sender);  // 预留
    void onExitButtonClicked(Ref* sender);      // 预留

    CREATE_FUNC(MenuScene);

private:
    // 你需要准备的png（Taffy提醒）
    // "menu_background.png"     // 全屏背景
    // "menu_logo.png"           // 游戏标题logo
    // "button_play_normal.png"  // 开始游戏按钮正常态
    // "button_play_pressed.png" // 开始游戏按钮按下态
    // "button_play_disabled.png"// 可选
};

Scene* MenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MenuScene::create();
    if (layer) {
        scene->addChild(layer);
    }
    return scene;
}

bool MenuScene::init()
{
    if (!Scene::init()) {  // 注意：这里其实是Layer的init，但因为我们继承Scene也没事
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 背景
    auto background = Sprite::create("menu_background.png");
    if (background) {
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        this->addChild(background, -1);
    }

    // Logo（可选）
    auto logo = Sprite::create("menu_logo.png");
    if (logo) {
        logo->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height * 0.75f + origin.y));
        this->addChild(logo, 0);
    }

    // 进度条背景
    auto progressBg = Sprite::create("loading_bar_bg.png");
    if (progressBg) {
        progressBg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height * 0.3f + origin.y));
        this->addChild(progressBg, 1);
    }

    // 进度条前景
    auto progressTimer = ProgressTimer::create(Sprite::create("loading_bar_fg.png"));
    if (progressTimer) {
        progressTimer->setType(ProgressTimer::Type::BAR);
        progressTimer->setMidpoint(Vec2(0, 0.5f));
        progressTimer->setBarChangeRate(Vec2(1, 0));
        progressTimer->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height * 0.3f + origin.y));
        this->addChild(progressTimer, 2);

        auto progressTo = ProgressTo::create(2.0f, 100.0f);  // 2秒假加载
        auto jump = CallFunc::create([]() {
            GameDirector::getInstance()->replaceScene(VillageScene::createScene());
            });
        progressTimer->runAction(Sequence::create(progressTo, jump, nullptr));
    }

    // Loading文字
    auto label = Label::createWithTTF("Loading...", "fonts/Marker Felt.ttf", 36);
    if (label) {
        label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height * 0.2f + origin.y));
        this->addChild(label, 3);
    }

    return true;
}