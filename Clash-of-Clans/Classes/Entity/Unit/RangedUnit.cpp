#include "RangedUnit.h"

USING_NS_CC;
const int arrow_damage = 30;
Sprite* RangedUnit::createSprite()
{
    // 默认图：弓箭手站立
    auto sprite = Sprite::create("AQ_Japan_Neutral_Shadow_01.png");

    if (sprite) 
    {
        sprite->setPosition(getPosition());
        // 我们在基类里建议过保存这个指针，方便换图
        _mySprite = sprite; 
    }
    return sprite;
}


void RangedUnit::executeAttack(int x, int y)
{
    // 远程单位不立刻扣血，而是产生一个箭矢
    SimpleAudioEngine::getInstance()->playEffect("audio/RangedUnit.mp3");// 播放攻击音效
    auto arrow = cocos2d::Sprite::create("arrow.png");
    arrow->setPosition(this->getPosition());
    this->_mySprite->getParent()->addChild(arrow);
    Vec2 targetWorldPos = GridUtils::gridToWorld(Vec2(x, y));
    float duration = getPosition().distance(targetWorldPos) / 500.0f; // 假设箭速500

    auto moveTo = cocos2d::MoveTo::create(duration, targetWorldPos);
    auto onHit = cocos2d::CallFunc::create([x, y, this, arrow]() 
    {
        // 箭到了，才真正改网格里的血量
        if (grid[x][y].now_health > 0) {
            grid[x][y].now_health -= arrow_damage;
            if (grid[x][y].now_health <= 0) grid[x][y].now_health = -1;
        }

        arrow->removeFromParent();
    });

    arrow->runAction(cocos2d::Sequence::create(moveTo, onHit, nullptr));
}

