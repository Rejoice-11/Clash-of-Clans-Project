#pragma once

#include "cocos2d.h"
#include<vector>
/**
 * @brief 相机/屏幕特效工具类
 */
class CameraUtils
{
    public:
        /**
         * @brief 执行震屏效果
         * @param target 震动的目标节点（通常传入当前运行的 Scene）
         * @param duration 持续时间（秒）
         * @param strength 震动强度（像素偏移量）
         */
        static void shakeScreen(cocos2d::Node* target, float duration, float strength);
    
    private:
        // 禁止实例化
        CameraUtils() = default;
};
