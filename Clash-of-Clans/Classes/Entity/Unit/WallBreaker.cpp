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
        target->takeDamage(this->getDamage());

        // 4. 播放爆炸特效（示例）
        if (_mySprite) {
            // 停止之前的移动动作 (如果有的话)
            _mySprite->stopAllActions();

            // 1. 换成爆炸效果图片 
            
            _mySprite->setTexture("bomb_explode.png");

            // 2. 创建动作序列：停留0.5秒 -> 移除自己
            auto delay = DelayTime::create(0.5f);

            // CallFunc 用来执行一些清理工作，最后 RemoveSelf 移除节点
            auto finishExplosion = CallFunc::create([this]() {
                // 这里的 this 实际上在逻辑层可能已经被管理类清理了
                // 但 Cocos 的 Node 还是存在的。
                // 通常这里只需要处理 Node 的移除。
                if (_mySprite) _mySprite->removeFromParent();
                });

            // 执行序列： 延时 -> 回调清理
            // 注意：因为 Unit 类本身可能是一个 Ref 或者由管理器 delete，
            // 这里的视觉层 _mySprite 应该独立于逻辑层 Unit 指针的生命周期管理
            // 简单做法：让 Sprite 执行 RemoveSelf
            auto remove = RemoveSelf::create();

            _mySprite->runAction(Sequence::create(delay, remove, nullptr));
        }

        // 5. 自爆：强制自身承受最大生命值的伤害，确保死亡
        this->takeDamage(this->getMaxHp());
    }
    else {
        // 6. 未到达范围，继续移动
        moveTowards(target->getPosition());
    }
}