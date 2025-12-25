#include "Classes/Utils/CameraUtils.h"

USING_NS_CC;

void CameraUtils::shakeScreen(Node* target, float duration, float strength)
{
    if (!target) return;

    // 1. 记录原始位置，确保震动结束后能准确回到原点
    Vec2 originalPos = target->getPosition();

    // 2. 计算震动频率（假设每秒震动 20 次）
    int shakeCount = static_cast<int>(duration * 20);
    if (shakeCount < 2) shakeCount = 2; // 至少震动两次

    float stepDuration = duration / shakeCount;

    // 3. 创建动作序列
    Vector<FiniteTimeAction*> actions;

    for (int i = 0; i < shakeCount; i++)
    {
        // 随机生成偏移向量
        // RandomHelper 是 Cocos2d-x 提供的随机工具
        float offX = RandomHelper::random_real(-strength, strength);
        float offY = RandomHelper::random_real(-strength, strength);

        // 移动到一个随机偏移点
        auto moveBy = MoveTo::create(stepDuration, originalPos + Vec2(offX, offY));
        actions.pushBack(moveBy);
    }

    // 4. 最后一个动作必须是回到原始位置
    actions.pushBack(MoveTo::create(stepDuration, originalPos));

    // 5. 执行序列动作
    target->runAction(Sequence::create(actions));
}