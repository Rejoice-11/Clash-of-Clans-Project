#ifndef CONSTANTS_H
#define CONSTANTS_H



//全局常量（BUILDING_TYPE_TOWN_HALL = 0）
namespace GameConfig 
{
    // 渲染层级
    static const int Z_BACKGROUND = -1;
    static const int Z_GRID = 4;
    static const int Z_BUILDING = 10;
    static const int Z_UNIT = 20;
    static const int Z_EFFECT = 30;
    static const int Z_UI = 100;

    // 战斗逻辑常量
    static const float DEPLOY_BOUNDARY_Y = 150.0f; // 兵种部署的 Y 轴分界线
}
#endif // !CONSTANTS_H