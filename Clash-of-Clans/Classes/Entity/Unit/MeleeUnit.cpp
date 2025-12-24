#include "Classes/Entity/Unit/MeleeUnit.h"

USING_NS_CC;

Sprite* MeleeUnit::createSprite() {
    auto sprite = Sprite::create("Troop_HV_Barbarian_28.png");
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
        
        target->takeDamage(_data.damage);
        CCLOG("Barbarian %d hits target for %d damage", getId(), _data.damage);
    }
    else {
        // 够不着，继续追击
        moveTowards(target->getPosition());
    }
}