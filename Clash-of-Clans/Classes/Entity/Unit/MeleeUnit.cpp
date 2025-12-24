#include "MeleeUnit.h"

USING_NS_CC;

Sprite* MeleeUnit::createSprite() {
    // 假设你的资源路径是 res/units/barbarian.png
    // 实际项目中可以根据 _data.id 来区分不同等级或兵种的图
    auto sprite = Sprite::create("units/barbarian.png");
    if (sprite) {
        sprite->setPosition(getPosition());
    }
    return sprite;
}

void MeleeUnit::attack(GameObject* target) {
    if (!target || isDead()) return;

    float dist = getPosition().distance(target->getPosition());

    // 近战单位必须在攻击范围内
    if (dist <= _data.attackRange) {
        // TODO: 播放挥剑动画
        // target->takeDamage(_data.damage);
        CCLOG("Barbarian %d hits target for %d damage", getId(), _data.damage);
    }
    else {
        // 够不着，继续追击
        moveTowards(target->getPosition());
    }
}