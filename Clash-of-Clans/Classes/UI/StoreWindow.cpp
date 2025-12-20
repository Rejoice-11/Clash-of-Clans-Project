#include "StoreWindow.h"

StoreWindow* StoreWindow::create(const std::function<void()>& closeCallback)
{
    auto window = new (std::nothrow) StoreWindow();
    // 先调用基类无参的init()，再调用自定义的带参初始化函数
    if (window && window->init() && window->initWithCloseCallback(closeCallback))
    {
        window->autorelease();
        return window;
    }
    CC_SAFE_DELETE(window);
    return nullptr;
}

// 自定义带参初始化函数（原来的init逻辑移到这里）
bool StoreWindow::initWithCloseCallback(const std::function<void()>& closeCallback)
{
    _closeCallback = closeCallback;
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();

    // 1. 遮罩层
    _grayMask = LayerColor::create(Color4B(0, 0, 0, 180), _visibleSize.width, _visibleSize.height);
    _grayMask->setPosition(_origin);
    _grayMask->setVisible(false);
    this->addChild(_grayMask, 10);

    // 2. 商店主面板
    _storePanel = Sprite::create("basic_market_bar.png");
    if (_storePanel)
    {
        auto centerPos = Vec2(_visibleSize.width / 2 + _origin.x, _visibleSize.height / 2 + _origin.y);
        _storePanel->setPosition(centerPos);
        _storePanel->setVisible(false);
        this->addChild(_storePanel, 20);

        // 关闭按钮
        auto closeBtn = MenuItemImage::create(
            "out_of_now.png", "out_of_now.png",
            CC_CALLBACK_1(StoreWindow::onCloseButtonClicked, this));
        closeBtn->setPosition(Vec2(
            _storePanel->getContentSize().width - 30,
            _storePanel->getContentSize().height - 30));

        auto menu = Menu::create(closeBtn, nullptr);
        menu->setPosition(Vec2::ZERO);
        _storePanel->addChild(menu, 1);
    }

    return true;
}

// 显示商店弹窗（同步修改自身visible状态）
void StoreWindow::show()
{
    // 1. 显示自身（Layer）
    this->setVisible(true);
    // 2. 显示内部面板/遮罩
    if (_grayMask) _grayMask->setVisible(true);
    if (_storePanel) _storePanel->setVisible(true);
    // 3. 开启触控吞掉
    this->setTouchEnabled(true);
    this->setSwallowsTouches(true);
}

// 隐藏商店弹窗（同步修改自身visible状态）
void StoreWindow::hide()
{
    // 1. 隐藏自身（Layer）
    this->setVisible(false);
    // 2. 隐藏内部面板/遮罩
    if (_grayMask) _grayMask->setVisible(false);
    if (_storePanel) _storePanel->setVisible(false);
    // 3. 关闭触控
    this->setTouchEnabled(false);
    this->setSwallowsTouches(false);
}
void StoreWindow::onCloseButtonClicked(Ref* sender)
{
    this->hide();
    if (_closeCallback)
    {
        _closeCallback();
    }
}