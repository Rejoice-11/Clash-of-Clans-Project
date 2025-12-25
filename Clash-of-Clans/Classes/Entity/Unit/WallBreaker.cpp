#include "WallBreaker.h"

USING_NS_CC;

Sprite* WallBreaker::createSprite() {
    auto sprite = Sprite::create("Troop_HV_Wall_Breaker_1.png");
    if (sprite) {
        sprite->setPosition(getPosition());
    }
    return sprite;
}

bool WallBreaker::prefersTarget(const Building* target) const {
    // 炸弹人只攻击未被摧毁的建筑
    return target && target->getState() != State::DESTROYED;
}

void WallBreaker::attack(GameObject* target) {
    if (!target || isDead()) return;

    // 1. 严格限制：炸弹人只攻击建筑，忽略其他单位
    if (target->getType() != GameObject::Type::BUILDING) {
        return;
    }

    float dist = getPosition().distance(target->getPosition());

    // 2. 判定是否到达引爆范围
    if (dist <= _data.attackRange) {
        CCLOG("WallBreaker %d is exploding on target %d!", getId(), target->getId());

        // 3. 造成伤害（模拟爆炸）
        // TODO: 如果需要 AOE（范围伤害），此处需获取游戏世界中的建筑列表，
        // 遍历 target 周围 distance <= explosionRadius 的所有建筑并调用 takeDamage。
        // 当前仅对锁定目标造成伤害。
        target->takeDamage(_data.damage);

        // 4. 播放爆炸特效（示例）
        if (_mySprite) {
            // 可以添加粒子特效或爆炸动画
            // auto explosion = ParticleExplosion::create();
            // _mySprite->getParent()->addChild(explosion);
        }

        // 5. 自爆：强制自身承受最大生命值的伤害，确保死亡
        this->takeDamage(this->getMaxHp());
    }
    else {
        // 6. 未到达范围，继续移动
        moveTowards(target->getPosition());
    }
}