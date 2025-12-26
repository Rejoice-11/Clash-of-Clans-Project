// scene/MenuScene.h
#pragma once

#include "cocos2d.h"
// 新增：引入音频引擎头文件
#include "audio/include/SimpleAudioEngine.h"
#include "Classes/Scene/MenuScene.h"
#include "Classes/Scene/VillageScene.h"
#include "Classes/Core/GameDirector.h"
#include "Classes/Core/ConfigManagerUnit.h"
USING_NS_CC;
// 新增：使用音频引擎命名空间
using namespace CocosDenshion;

class MenuScene : public Scene
{
public:
    static Scene* createScene();
    virtual bool init() override;

    // 新增：重写onExit函数，确保场景销毁时释放音频
    virtual void onExit() override;

    // 按钮回调
    void onPlayButtonClicked(Ref* sender);
    void onSettingsButtonClicked(Ref* sender);
    void onExitButtonClicked(Ref* sender);

    CREATE_FUNC(MenuScene);

private:
    // 你需要准备的png
    // "menu_background.png"     // 全屏背景
    // "menu_logo.png"           // 游戏标题logo
    // "button_play_normal.png"  // 开始游戏按钮正常态
    // "button_play_pressed.png" // 开始游戏按钮按下态
    // "button_play_disabled.png"// 可选

    // 新增：音乐文件名常量（建议放在配置文件中）
    const std::string MENU_BGM = "audio/menu_bgm.mp3"; // 根据你的实际路径修改
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
    if (!Scene::init()) {
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

        // 新增：加载完成的回调函数（单独抽离，方便维护）
        auto onLoadComplete = CallFunc::create([]() {
            // 停止背景音乐播放
            SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            // 跳转到村庄场景
            GameDirector::getInstance()->replaceScene(VillageScene::createScene());
            });

        auto progressTo = ProgressTo::create(2.0f, 100.0f);  // 2秒假加载
        progressTimer->runAction(Sequence::create(progressTo, onLoadComplete, nullptr));
    }

    // Loading文字
    auto label = Label::createWithTTF("Loading...", "fonts/Marker Felt.ttf", 36);
    if (label) {
        label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height * 0.2f + origin.y));
        this->addChild(label, 3);
    }

    // 新增：播放背景音乐
    auto audioEngine = SimpleAudioEngine::getInstance();
    // 设置背景音乐音量（0.0-1.0）
    audioEngine->setBackgroundMusicVolume(0.8f);
    // 循环播放背景音乐（第二个参数true表示循环）
    audioEngine->playBackgroundMusic(MENU_BGM.c_str(), true);

    return true;
}

// 新增：重写onExit函数，确保场景销毁时释放音频资源
void MenuScene::onExit()
{
    Scene::onExit();
    // 可选：释放音频引擎资源（如果整个游戏退出时再调用更合适）
    // SimpleAudioEngine::getInstance()->end();
}

// 空实现（保留你的原有函数）
void MenuScene::onPlayButtonClicked(Ref* sender) {}
void MenuScene::onSettingsButtonClicked(Ref* sender) {}
void MenuScene::onExitButtonClicked(Ref* sender) {}