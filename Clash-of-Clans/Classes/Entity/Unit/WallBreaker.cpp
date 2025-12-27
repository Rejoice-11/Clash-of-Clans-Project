#include "WallBreaker.h"

USING_NS_CC;

Sprite* WallBreaker::createSprite() {
    auto sprite = Sprite::create("Troop_HV_Wall_Breaker_1.png");
    if (sprite) {
        sprite->setPosition(getPosition());
        _mySprite = sprite; // 关联到基类的成员变量
    }
    return sprite;
}


void WallBreaker::executeAttack(int x, int y) {
    // 1. 造成巨大伤害
    SimpleAudioEngine::getInstance()->playEffect("audio/WallBreaker.mp3");// 播放攻击音效
    grid[x][y].now_health -= getDamage();
    if (grid[x][y].now_health <= 0) grid[x][y].now_health = -1;

    // 2. 视觉效果：切换爆炸纹理并延迟移除
    if (_mySprite) {
        _mySprite->setTexture("bomb_explode.png");
        auto delay = cocos2d::DelayTime::create(0.5f);
        auto remove = cocos2d::RemoveSelf::create();
        _mySprite->runAction(cocos2d::Sequence::create(delay, remove, nullptr));
    }

    // 3. 自爆逻辑：逻辑上让自己立刻死亡
    this->takeDamage(this->getMaxHp());
}

