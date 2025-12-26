#include "GridUtils.h"

// 初始化静态成员
const Vec2 GridUtils::LEFT_VERTEX = Vec2(256, 392);
const Vec2 GridUtils::RIGHT_VERTEX = Vec2(1056, 392);  // 计算得出的右顶点
const Vec2 GridUtils::TOP_VERTEX = Vec2(656, 712);
const Vec2 GridUtils::BOTTOM_VERTEX = Vec2(656, 72);

float GridUtils::_gridWidth = 0;
float GridUtils::_gridHeight = 0;
Vec2 GridUtils::_gridOrigin = Vec2::ZERO;
bool GridUtils::_parametersInitialized = false;

void GridUtils::initGridParameters()
{
    if (_parametersInitialized) return;

    // 计算大菱形的宽度和高度
    float diamondWidth = distance(LEFT_VERTEX, RIGHT_VERTEX);
    float diamondHeight = distance(TOP_VERTEX, BOTTOM_VERTEX);

    // 计算每个小菱形的尺寸
    _gridWidth = diamondWidth / GRID_COLS;
    _gridHeight = diamondHeight / GRID_ROWS;

    // 计算网格原点(左上角第一个格子的中心)
    _gridOrigin = Vec2(LEFT_VERTEX.x, TOP_VERTEX.y);

    _parametersInitialized = true;
}

float GridUtils::distance(const Vec2& a, const Vec2& b)
{
    return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2));
}

float GridUtils::pointToLineDistance(const Vec2& point, const Vec2& lineStart, const Vec2& lineEnd)
{
    float A = point.x - lineStart.x;
    float B = point.y - lineStart.y;
    float C = lineEnd.x - lineStart.x;
    float D = lineEnd.y - lineStart.y;

    float dot = A * C + B * D;
    float lenSq = C * C + D * D;
    float param = -1;

    if (lenSq != 0)
        param = dot / lenSq;

    float xx, yy;

    if (param < 0)
    {
        xx = lineStart.x;
        yy = lineStart.y;
    }
    else if (param > 1)
    {
        xx = lineEnd.x;
        yy = lineEnd.y;
    }
    else
    {
        xx = lineStart.x + param * C;
        yy = lineStart.y + param * D;
    }

    float dx = point.x - xx;
    float dy = point.y - yy;

    return sqrtf(dx * dx + dy * dy);
}

void GridUtils::drawGrid(Node* parent)
{
    initGridParameters();

    // 创建绘制命令
    auto drawNode = DrawNode::create();
    parent->addChild(drawNode, GameConfig::Z_GRID);  // 确保在背景之上，建筑之下

    // 关键修改：将世界坐标转换为父节点（_scrollNode）的本地坐标
    Vec2 left = parent->convertToNodeSpace(LEFT_VERTEX);
    Vec2 right = parent->convertToNodeSpace(RIGHT_VERTEX);
    Vec2 top = parent->convertToNodeSpace(TOP_VERTEX);
    Vec2 bottom = parent->convertToNodeSpace(BOTTOM_VERTEX);
    // 绘制大菱形边框（使用转换后的本地坐标）
    drawNode->drawLine(left, top, Color4F::GRAY);
    drawNode->drawLine(top, right, Color4F::GRAY);
    drawNode->drawLine(right, bottom, Color4F::GRAY);
    drawNode->drawLine(bottom, left, Color4F::GRAY);
    // 绘制小网格线（同样需要转换坐标）
    Color4F gridColor(0.5f, 0.5f, 0.5f, 0.5f);

    // 水平网格线
    for (int i = 0; i <= GRID_ROWS; ++i)
    {
        float ratio = (float)i / GRID_ROWS;
        Vec2 leftPoint = LEFT_VERTEX.lerp(BOTTOM_VERTEX, ratio);
        Vec2 rightPoint = TOP_VERTEX.lerp(RIGHT_VERTEX, ratio);
        // 转换为父节点本地坐标
        leftPoint = parent->convertToNodeSpace(leftPoint);
        rightPoint = parent->convertToNodeSpace(rightPoint);
        drawNode->drawLine(leftPoint, rightPoint, gridColor);
    }

    // 垂直网格线
    for (int i = 0; i <= GRID_COLS; ++i)
    {
        float ratio = (float)i / GRID_COLS;
        Vec2 topPoint = LEFT_VERTEX.lerp(TOP_VERTEX, ratio);
        Vec2 bottomPoint = BOTTOM_VERTEX.lerp(RIGHT_VERTEX, ratio);
        // 转换为父节点本地坐标
        topPoint = parent->convertToNodeSpace(topPoint);
        bottomPoint = parent->convertToNodeSpace(bottomPoint);
        drawNode->drawLine(topPoint, bottomPoint, gridColor);
    }
}

bool GridUtils::isPointInDiamondBattle(const Vec2& point)
{
    
	// 1. 计算菱形的中心点 (利用你的顶点坐标)
	float centerX = (LEFT_VERTEX.x + RIGHT_VERTEX.x) / 2.0f; // 应该是 656
	float centerY = (TOP_VERTEX.y + BOTTOM_VERTEX.y) / 2.0f; // 应该是 392

	// 2. 计算菱形的半宽和半高
	float halfWidth = (RIGHT_VERTEX.x - LEFT_VERTEX.x) / 2.0f; // 400
	float halfHeight = (TOP_VERTEX.y - BOTTOM_VERTEX.y) / 2.0f; // 320

	// 3. 将点击的点相对于中心点进行绝对值化
	float dx = std::abs(point.x - centerX);
	float dy = std::abs(point.y - centerY);

	// 4. 菱形判定公式： |dx|/W + |dy|/H <= 1
	// 我们稍微给一点点容错（1.05），防止点在边界上判不中
	return (dx / halfWidth + dy / halfHeight) <= 1.05f;

}


bool GridUtils::isPointInDiamond(const Vec2& point)
{
    
    // 计算点到四条边的距离之和是否小于等于大菱形的半周长
    float distanceToLeftEdge = pointToLineDistance(point, LEFT_VERTEX, TOP_VERTEX);
    float distanceToTopEdge = pointToLineDistance(point, TOP_VERTEX, RIGHT_VERTEX);
    float distanceToRightEdge = pointToLineDistance(point, RIGHT_VERTEX, BOTTOM_VERTEX);
    float distanceToBottomEdge = pointToLineDistance(point, BOTTOM_VERTEX, LEFT_VERTEX);

    // 计算大菱形的半高和半宽
    float halfHeight = distance(TOP_VERTEX, BOTTOM_VERTEX) / 2;
    float halfWidth = distance(LEFT_VERTEX, RIGHT_VERTEX) / 2;

    // 点在菱形内的判定
    return (distanceToLeftEdge + distanceToRightEdge <= halfWidth * 1.05f &&
        distanceToTopEdge + distanceToBottomEdge <= halfHeight * 1.05f);
    
}

Vec2 GridUtils::snapToGrid(const Vec2& point)
{
    initGridParameters();

    // 先转换为网格坐标
    Vec2 gridPos = worldToGrid(point);

    // 取整
    gridPos.x = roundf(gridPos.x);
    gridPos.y = roundf(gridPos.y);

    // 限制在网格范围内
    gridPos.x = clampf(gridPos.x, 0, GRID_COLS - 1);
    gridPos.y = clampf(gridPos.y, 0, GRID_ROWS - 1);

    // 转换回世界坐标
    return gridToWorld(gridPos);
}

bool GridUtils::isBuildingInDiamond(const Vec2& gridPos)
{
    // 检查3x3建筑的所有格子是否都在大菱形内
    for (int i = 0; i < BUILDING_SIZE; ++i)
    {
        for (int j = 0; j < BUILDING_SIZE; ++j)
        {
            int x = gridPos.x + i - BUILDING_SIZE / 2;
            int y = gridPos.y + j - BUILDING_SIZE / 2;

            // 检查是否超出网格范围
            if (x < 1 || x >= GRID_COLS + 1 || y < 1 || y >= GRID_ROWS + 1)
                return false;
        }
    }
    return true;
}

Vec2 GridUtils::gridToWorld(const Vec2& gridPos)
{
    initGridParameters();

    float x = (gridPos.x + gridPos.y - 1.0f) * (0.5f * _gridWidth) + LEFT_VERTEX.x;

    float y = (gridPos.y - gridPos.x) * (0.5f * _gridHeight) + LEFT_VERTEX.y;

    return Vec2(x, y);
}

Vec2 GridUtils::worldToGrid(const Vec2& worldPos)
{
    initGridParameters();

    float dx = worldPos.x;
    float dy = worldPos.y;


    float col = (dx * 0.5f - dy * 0.625f + 373.0f - LEFT_VERTEX.x) / (_gridWidth * 0.5) + 1.0f;
    float row = (dx * 0.4f + dy * 0.5f + 93.6f - LEFT_VERTEX.y) / (_gridHeight * 0.5) + 1.0f;
    return Vec2(col, row);
}