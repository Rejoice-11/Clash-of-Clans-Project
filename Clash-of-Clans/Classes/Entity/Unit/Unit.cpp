#include "Unit.h"
#include <algorithm>


// 承伤逻辑：扣血并检查死亡
void Unit::takeDamage(int damage) {
    if (isDead()) return;

    int newHp = _hp - damage;
    setHp(newHp); // 利用你写好的 setHp 进行 0 值保护

    if (isDead()) {
        setState(State::DESTROYED); // 修改 GameObject 里的状态
        if (_mySprite) {
            // 优雅的消失逻辑：先变透明，再移除
            auto fadeOut = cocos2d::FadeOut::create(0.5f); // 0.5秒淡出
            auto remove = cocos2d::RemoveSelf::create();  // 自动把自己从场景移除
            _mySprite->runAction(cocos2d::Sequence::create(fadeOut, remove, nullptr));
        }

        CCLOG("Unit %d has been destroyed.", getId());
    }
}

// 攻击偏好：默认实现为攻击任何未被摧毁的建筑
bool Unit::prefersTarget(const Building* target) const {
    return target && target->getState() != State::DESTROYED;
}

// 基础移动：向目标向量线性移动
void Unit::moveTowards(const cocos2d::Vec2& target) {
    if (isDead()) return;

    cocos2d::Vec2 currentPos = getPosition();
    cocos2d::Vec2 direction = target - currentPos;
    float distance = direction.length();

    if (distance > 1.0f) { // 距离大于1像素才移动，防止抖动
        direction.normalize();
        // 假设 dt 为 1/60 秒，实际应从 update 传进来
        float dt = 0.01667f;
        cocos2d::Vec2 moveVec = direction * _data.speed * dt;

        // 如果步长超过了距离，直接到达
        if (moveVec.length() > distance) {
            setPosition(target);
        }
        else {
            setPosition(currentPos + moveVec);
        }
    }
}

// 基础攻击：直接对目标调用受损逻辑
void Unit::attack(GameObject* target) {
    if (!target || isDead() || target->getState() == State::DESTROYED) return;
 
    target->takeDamage(_data.damage); 

    CCLOG("Unit %d is dealing %d damage", getId(), _data.damage);
}