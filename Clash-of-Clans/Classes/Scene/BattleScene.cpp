// 必须包含对应的头文件
#include "BattleScene.h"

// 实现 createScene 静态函数
Scene* BattleScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BattleScene::create();
    if (layer) {
        scene->addChild(layer);
    }
    return scene;
}

// 实现 init 初始化函数
bool BattleScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 1. 战斗背景（全屏铺开）
    auto background = Sprite::create("village_background.png");
    if (background) {
        // 等比缩放至填满屏幕
        float scaleX = visibleSize.width / background->getContentSize().width;
        float scaleY = visibleSize.height / background->getContentSize().height;
        float scale = MAX(scaleX, scaleY);
        background->setScale(scale);

        background->setPosition(Vec2(visibleSize.width / 2 + origin.x,
            visibleSize.height / 2 + origin.y));
        this->addChild(background, -1);
    }

    // 2. 临时提示文字（验证场景跳转）
    auto label = Label::createWithTTF("Battle Scene!\nTaffy的屠村时间到咯~",
        "fonts/Marker Felt.ttf", 48);
    if (label) {
        label->setPosition(Vec2(visibleSize.width / 2 + origin.x,
            visibleSize.height / 2 + origin.y));
        label->setColor(Color3B::RED);
        this->addChild(label, 10);
    }

    // 开启帧更新（后续战斗逻辑靠这个）
    this->scheduleUpdate();

    return true;
}

// 实现 update 帧更新函数
void BattleScene::update(float dt)
{
    // 暂时预留，后续添加：部队寻路、伤害结算、胜利判定等战斗逻辑
}

// 未实现的按钮回调（保持代码完整性）
void BattleScene::onSurrenderButtonClicked(Ref* sender) {}
void BattleScene::onNextWaveButtonClicked(Ref* sender) {}