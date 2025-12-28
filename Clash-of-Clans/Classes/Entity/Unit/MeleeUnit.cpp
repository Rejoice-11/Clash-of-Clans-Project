#include "Classes/Entity/Unit/MeleeUnit.h"

USING_NS_CC;

Sprite* MeleeUnit::createSprite() 
{
    auto sprite = Sprite::create("Troop_HV_Barbarian_28.png");

    if (sprite) 
    {
        sprite->setPosition(getPosition());
        _mySprite = sprite; // 关联到基类的成员变量
    }

    return sprite;
}


void MeleeUnit::executeAttack(int x, int y)
{
    // 1. 安全检查（防止索引越界）
    if (x < 0 || x >= 41 || y < 0 || y >= 41)
        return;

    // 2. 只有目标还活着才攻击
    if (grid[x][y].now_health > 0)
    {
        SimpleAudioEngine::getInstance()->playEffect("audio/MeleeUnit.mp3");// 播放攻击音效
        // 3. 执行扣血
        grid[x][y].now_health -= this->getDamage();

        CCLOG("Barbarian %d hits grid(%d, %d). HP left: %d", getId(), x, y, grid[x][y].now_health);

        // 4. 死亡判定：摧毁置为 -1
        if (grid[x][y].now_health <= 0) 
        {
            grid[x][y].now_health = -1;
            CCLOG("Barbarian destroyed building at (%d, %d)!", x, y);
        }
        
        // 5. 简单的视觉反馈：让小兵在攻击时稍微变大一点点再缩回去，模拟“发力”动作
        if (_mySprite) 
        {
            auto scaleUp = cocos2d::ScaleTo::create(0.1f, 1.2f);
            auto scaleDown = cocos2d::ScaleTo::create(0.1f, 1.0f);
            _mySprite->runAction(cocos2d::Sequence::create(scaleUp, scaleDown, nullptr));
        }
    }
}
