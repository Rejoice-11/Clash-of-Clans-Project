//游戏状态机（菜单→村庄→战斗） 
// core/GameDirector.cpp
#include "core/GameDirector.h"

GameDirector* GameDirector::instance = nullptr;

GameDirector* GameDirector::getInstance()
{
    if (!instance) {
        instance = new (std::nothrow) GameDirector();
    }
    return instance;
}

void GameDirector::init()
{
    // 这里以后可以放全局初始化（ConfigManager加载等）
    currentState = GameState::MENU;
    CCLOG("GameDirector initialized! Current state: MENU");
}

void GameDirector::runWithScene(Scene* scene)
{
    auto director = Director::getInstance();
    if (director->getRunningScene()) {
        director->replaceScene(TransitionFade::create(0.5f, scene));
    }
    else {
        director->runWithScene(TransitionFade::create(0.5f, scene));
    }
}

void GameDirector::replaceScene(Scene* scene)
{
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
    // 根据场景类型自动更新状态（可扩展）
    // 比如如果scene是VillageScene就设VILLAGE
}

void GameDirector::pushScene(Scene* scene)
{
    Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
}

void GameDirector::popScene()
{
    Director::getInstance()->popScene();
}