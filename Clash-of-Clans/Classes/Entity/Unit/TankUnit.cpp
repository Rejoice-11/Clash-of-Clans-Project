#include "Classes/Entity/Unit/TankUnit.h"

USING_NS_CC;

Sprite* TankUnit::createSprite() 
{
    // 假设巨人的资源文件名为这个
    auto sprite = Sprite::create("Giant_lvl_14.png");
    if (sprite) 
    {
        sprite->setPosition(getPosition());
        // 巨人通常体积较大，如果需要可以在这里设置缩放
        sprite->setScale(1.2f); 
        _mySprite = sprite; // 关联到基类的成员变量
    }
    return sprite;
}


void TankUnit::findBestTarget()
{
    float minSourceDist = 999999.0f;
    Vec2 myGridPos = GridUtils::worldToGrid(getPosition());
    Vec2 bestDefense = Vec2(-1, -1);
    Vec2 bestAny = Vec2(-1, -1);
    float minAnyDist = 999999.0f;

    for (int i = 0; i < 41; ++i) 
    {
        for (int j = 0; j < 41; ++j) 
        {
            if (grid[i][j].now_health > 0) 
            {
                float d = myGridPos.distance(Vec2(i, j));

                // 优先寻找防御建筑 (假设 buildingtype == 3 是防御)
                if (grid[i][j].buildingtype == 3|| grid[i][j].buildingtype == 4) 
                {
                    if (d < minSourceDist)
                    {
                        minSourceDist = d;
                        bestDefense = Vec2(i, j);
                    }
                }
                // 同时记录最近的普通建筑，以防防御建筑全拆完了
                if (d < minAnyDist) 
                {
                    minAnyDist = d;
                    bestAny = Vec2(i, j);
                }
            }
        }
    }

    // 巨人逻辑：有防御打防御，没防御打最近的建筑
    _targetGridPos = (bestDefense.x != -1) ? bestDefense : bestAny;
}


void TankUnit::executeAttack(int x, int y) 
{
    // 造成伤害
    SimpleAudioEngine::getInstance()->playEffect("audio/TankUnit.mp3");// 播放攻击音效
    grid[x][y].now_health -= getDamage();
    if (grid[x][y].now_health <= 0) grid[x][y].now_health = -1;

    // 特有效果：震屏
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    CameraUtils::shakeScreen(scene, 0.2f, 6.0f);

    CCLOG("Giant is smashing grid (%d, %d)", x, y);
}