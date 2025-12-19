//游戏状态机（菜单→村庄→战斗）
// core/GameDirector.h
#pragma once

#include "cocos2d.h"
USING_NS_CC;

class GameDirector
{
public:
    // 游戏状态枚举
    enum class GameState {
        MENU,
        VILLAGE,
        BATTLE,
        LOADING
    };

    static GameDirector* getInstance();
    void init();

    // 场景控制函数
    void runWithScene(Scene* scene);
    void replaceScene(Scene* scene);           // 带淡入淡出过渡
    void pushScene(Scene* scene);              // 压栈（子界面用）
    void popScene();                           // 出栈

    // 获取/设置当前状态（方便其他模块查询）
    GameState getCurrentState() const { return currentState; }
    void setCurrentState(GameState state) { currentState = state; }

private:
    GameDirector() = default;
    ~GameDirector() = default;
    GameDirector(const GameDirector&) = delete;
    GameDirector& operator=(const GameDirector&) = delete;

    static GameDirector* instance;
    GameState currentState = GameState::MENU;  // 默认菜单状态
};