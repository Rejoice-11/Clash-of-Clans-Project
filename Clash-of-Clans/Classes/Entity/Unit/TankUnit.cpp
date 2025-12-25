#include "Classes/Entity/Unit/TankUnit.h"

USING_NS_CC;

Sprite* TankUnit::createSprite() {
    // 假设巨人的资源文件名为这个
    auto sprite = Sprite::create("Troop_HV_Giant_28.png");
    if (sprite) {
        sprite->setPosition(getPosition());
        // 巨人通常体积较大，如果需要可以在这里设置缩放
        sprite->setScale(1.2f); 
        _mySprite = sprite; // 关联到基类的成员变量
    }
    return sprite;
}

bool TankUnit::prefersTarget(const Building* target) const {
    // 巨人的逻辑：只攻击建筑且建筑必须是存活状态
    // 在更复杂的逻辑中，这里可以判断 target->getBuildingType() == DEFENSE
    return target && target->getState() != State::DESTROYED;
}

void TankUnit::attack(GameObject* target) {
	if (!target || isDead() || target->getState() == State::DESTROYED || !canAttack()) return;

    // 检查目标是否为建筑（巨人的天职）
    if (target->getType() != GameObject::Type::BUILDING) {
        return; // 不攻击非建筑单位（如敌方士兵）
    }

    float dist = getPosition().distance(target->getPosition());

    // 坦克通常也是近战，需要在攻击范围内
    if (dist <= _data.attackRange) {
        // 执行伤害
		target->takeDamage(this->getDamage());
		resetAttackTimer(); // 重置攻击计时器
        // 可以在这里添加巨人特有的攻击特效或震屏逻辑

        auto currentScene = Director::getInstance()->getRunningScene();

        // 参数：场景节点，持续0.2秒，强度6像素
        CameraUtils::shakeScreen(currentScene, 0.2f, 6.0f);

        CCLOG("Giant %d crushing building for %d damage", getId(), _data.damage);
    }
    else {
        // 够不着，继续向建筑移动
        moveTowards(target->getPosition());
    }
}