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
//不用了
/*bool Unit::prefersTarget(const Building* target) const {
    return target && target->getState() != State::DESTROYED;
}*/

// 在 Unit.cpp 中实现，作为默认逻辑
void Unit::findBestTarget() {
    float minSourceDist = 999999.0f;
    Vec2 myGridPos = GridUtils::worldToGrid(getPosition());
    Vec2 bestTarget = Vec2(-1, -1);

    for (int i = 0; i < 41; ++i) {
        for (int j = 0; j < 41; ++j) {
            // 只要有建筑且没被摧毁 (buildingtype > 0 且 now_health > 0)
            if (grid[i][j].buildingtype > 0 && grid[i][j].now_health > 0) {
                float d = myGridPos.distance(Vec2(i, j));
                if (d < minSourceDist) {
                    minSourceDist = d;
                    bestTarget = Vec2(i, j);
                }
            }
        }
    }
    _targetGridPos = bestTarget;
    if (_targetGridPos.x != -1) {
        CCLOG("Unit found nearest building at (%d, %d)", (int)bestTarget.x, (int)bestTarget.y);
    }
}


// 基础移动：向目标向量线性移动
void Unit::moveTowards(const cocos2d::Vec2& target, float dt) {
    if (isDead()) return;

    cocos2d::Vec2 currentPos = getPosition();
    cocos2d::Vec2 direction = target - currentPos;
    float distance = direction.length();

    if (distance > 2.0f) { // 距离大于1像素才移动，防止抖动
        direction.normalize();
        // 使用真正的 dt 计算步长
        cocos2d::Vec2 moveVec = direction * _data.speed * dt;

        // 如果步长超过了距离，直接到达
        if (moveVec.length() > distance) {
            setPosition(target);
        }
        else {
            setPosition(currentPos + moveVec);
        }

        // 让 Sprite 永远跟随着逻辑坐标移动
        if (_mySprite) {
            _mySprite->setPosition(getPosition());
        }
    }
}

// 每帧更新：累积攻击计时器
void Unit::update(float dt) {
    if (isDead()) return;
    _attackTimer += dt;

    // 1. 如果没有目标或目标已死，寻找新目标
    int tx = (int)_targetGridPos.x;
    int ty = (int)_targetGridPos.y;
    if (tx == -1 || grid[tx][ty].now_health <= 0) {
        findBestTarget();
        return;
    }

    // 2. 距离判断
    Vec2 targetWorldPos = GridUtils::gridToWorld(_targetGridPos);
    float dist = getPosition().distance(targetWorldPos);

    if (dist <= getAttackRange()) {
        // 3. 够到了：调用子类的具体攻击动作
        if (canAttack()) {
            this->executeAttack(tx, ty); // 执行子类特有的攻击逻辑
            resetAttackTimer();
        }
    }
    else {
        // 4. 够不到：执行移动（基类统一控制，子类不再写这部分）
        moveTowards(targetWorldPos, dt);
    }


}


// 基础攻击：直接对目标调用受损逻辑
//不用了
/*void Unit::attack(GameObject* target) {
	if (!target || isDead() || target->getState() == State::DESTROYED || !canAttack()) return;
 
    target->takeDamage(_data.damage); 

    CCLOG("Unit %d is dealing %d damage", getId(), _data.damage);
}*/