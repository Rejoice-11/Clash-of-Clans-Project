#include "Classes/Utils/CameraUtils.h"

USING_NS_CC;


// 定义一个全局或静态变量来记录真正的原点
static Vec2 s_actualOriginalPos = Vec2::ZERO;
static bool s_isShaking = false;
static const int SHAKE_ACTION_TAG = 998; // 给震动动作起个唯一 ID

void CameraUtils::shakeScreen(Node* target, float duration, float strength)
{
    if (!target) return;

    // 1. 核心逻辑：如果当前没有在震动，才记录原点
    // 如果正在震动，说明当前的 target->getPosition() 是歪的，不能记录！
    if (target->getActionByTag(SHAKE_ACTION_TAG) == nullptr)
    {
        s_actualOriginalPos = target->getPosition();
    }

    else 
    {
        // 如果正在震动，先停止旧的震动，准备开始新的（叠加感）
        target->stopActionByTag(SHAKE_ACTION_TAG);
    }

    // 2. 计算参数
    int shakeCount = static_cast<int>(duration * 20);
    if (shakeCount < 2) shakeCount = 2;
    float stepDuration = duration / shakeCount;

    Vector<FiniteTimeAction*> actions;

    for (int i = 0; i < shakeCount; i++)
    {
        float offX = RandomHelper::random_real(-strength, strength);
        float offY = RandomHelper::random_real(-strength, strength);

        // 注意：永远相对于 s_actualOriginalPos 进行偏移
        auto move = MoveTo::create(stepDuration, s_actualOriginalPos + Vec2(offX, offY));
        actions.pushBack(move);
    }

    // 3. 彻底复位
    actions.pushBack(MoveTo::create(stepDuration, s_actualOriginalPos));

    // 4. 执行并打上 Tag
    auto sequence = Sequence::create(actions);
    sequence->setTag(SHAKE_ACTION_TAG); // 重点：打上标签
    target->runAction(sequence);
}