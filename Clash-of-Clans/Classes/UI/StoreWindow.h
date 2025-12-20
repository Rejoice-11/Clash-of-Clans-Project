#pragma once

#include "cocos2d.h"
USING_NS_CC;

class StoreWindow : public Layer
{
public:
    static StoreWindow* create(const std::function<void()>& closeCallback);

    virtual bool initWithCloseCallback(const std::function<void()>& closeCallback);

    void show();
    void hide();
    void onCloseButtonClicked(Ref* sender);

private:
    Size _visibleSize;
    Vec2 _origin;
    LayerColor* _grayMask;
    Node* _storePanel;
    std::function<void()> _closeCallback;
};