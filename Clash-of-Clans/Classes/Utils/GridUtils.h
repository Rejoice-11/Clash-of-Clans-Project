//网格坐标转换（像素↔格子）
#pragma once
#include "cocos2d.h"
#include "Classes/Utils/Constants.h"
USING_NS_CC;

class GridUtils
{
public:
    // 大菱形四个顶点
    static const Vec2 LEFT_VERTEX;    // 左顶点
    static const Vec2 RIGHT_VERTEX;   // 右顶点
    static const Vec2 TOP_VERTEX;     // 上顶点
    static const Vec2 BOTTOM_VERTEX;  // 下顶点

    // 网格属性
    static const int GRID_ROWS = 40;  // 行数
    static const int GRID_COLS = 40;  // 列数
    static const int BUILDING_SIZE = 3; // 建筑占据的格子大小(3x3)

    // 绘制网格
    static void drawGrid(Node* parent);

    // 检查点是否在大菱形范围内
    static bool isPointInDiamond(const Vec2& point);

    // 将点吸附到最近的网格点
    static Vec2 snapToGrid(const Vec2& point);

    // 检查建筑(3x3)是否完全在大菱形范围内
    static bool isBuildingInDiamond(const Vec2& gridPos);

    // 将网格坐标转换为世界坐标
    static Vec2 gridToWorld(const Vec2& gridPos);

    // 将世界坐标转换为网格坐标
    static Vec2 worldToGrid(const Vec2& worldPos);

private:
    // 计算两个点之间的距离
    static float distance(const Vec2& a, const Vec2& b);

    // 计算点到直线的距离
    static float pointToLineDistance(const Vec2& point, const Vec2& lineStart, const Vec2& lineEnd);

    // 初始化网格参数
    static void initGridParameters();

    // 网格参数
    static float _gridWidth;    // 每个小菱形的宽度
    static float _gridHeight;   // 每个小菱形的高度
    static Vec2 _gridOrigin;    // 网格原点(左上角第一个格子的中心)
    static bool _parametersInitialized;
};