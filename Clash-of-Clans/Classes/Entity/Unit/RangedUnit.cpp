#include "RangedUnit.h"

USING_NS_CC;

Sprite* RangedUnit::createSprite() {
    // 默认图：弓箭手站立
    auto sprite = Sprite::create("AQ_Japan_Neutral_Shadow_01.png");
    if (sprite) {
        sprite->setPosition(getPosition());
        // 我们在基类里建议过保存这个指针，方便换图
        // _mySprite = sprite; 
    }
    return sprite;
}

void RangedUnit::attack(GameObject* target) {
    if (!target || isDead() || target->getState() == State::DESTROYED) return;

    float dist = getPosition().getDistance(target->getPosition());

    // 远程单位：只要在射程内就停止移动并开火
    if (dist <= _data.attackRange) {
        fireProjectile(target);
    }
    else {
        // 够不着，向目标走
        moveTowards(target->getPosition());
    }
}

void RangedUnit::fireProjectile(GameObject* target) {
    // 1. 创建箭矢精灵
    auto arrow = Sprite::create("arrow.png");
    if (!arrow || !target) return;

    // 2. 箭矢初始位置在弓箭手这里
    arrow->setPosition(this->getPosition());

    // 将箭矢添加到当前的场景中（假设精灵的父节点是层）
    _mySprite->getParent()->addChild(arrow);

    Vec2 direction = target->getPosition() - this->getPosition();//计算距离

    //  飞行并造成伤害
    float flySpeed = 500.0f; // 箭速：像素/秒
    float duration = direction.length() / flySpeed;

    auto moveTo = MoveTo::create(duration, target->getPosition());
    auto hitTarget = CallFunc::create([this, target, arrow]() {
        // 只有箭到了，目标才真正扣血
        if (target && target->getState() != State::DESTROYED) {
            target->takeDamage(this->getDamage()); // 使用 UnitData 里的伤害值
        }
        // 箭矢消失
        arrow->removeFromParent();
        });

    arrow->runAction(Sequence::create(moveTo, hitTarget, nullptr));
}