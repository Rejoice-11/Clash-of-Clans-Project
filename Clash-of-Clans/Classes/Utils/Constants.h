//全局常量（BUILDING_TYPE_TOWN_HALL = 0）
#pragma once
namespace GameConfig {
    // 渲染层级
    constexpr int Z_BACKGROUND = -1;
    constexpr int Z_GRID = 4;
    constexpr int Z_BUILDING = 10;
    constexpr int Z_UNIT = 20;
    constexpr int Z_EFFECT = 30;
    constexpr int Z_UI = 100;

    // 战斗逻辑常量
    constexpr float DEPLOY_BOUNDARY_Y = 150.0f; // 兵种部署的 Y 轴分界线
}