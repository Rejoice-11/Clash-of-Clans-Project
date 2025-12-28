#include "SettingLayer.h"

bool SettingLayer::init()
{
    if (!Layer::init()) {
        return false;
    }

    // 获取音频引擎实例
    _audioEngine = SimpleAudioEngine::getInstance();

    // 初始化音量值
    _bgMusicVolume = 0.5f;
    _soundEffectVolume = 0.8f;

    // 创建半透明黑色背景
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto bg = LayerColor::create(Color4B(0, 0, 0, 180), visibleSize.width, visibleSize.height);
    bg->setPosition(origin);
    this->addChild(bg);

    // 初始化UI
    initUI();

    // 加载音频设置
    loadAudioSettings();

    

    // 默认隐藏
    this->setVisible(false);

    return true;
}

void SettingLayer::initUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 创建设置面板背景
    _panelBg = Sprite::create("ui/setting_bg.png");
    if (!_panelBg) {
        _panelBg = Sprite::create();
        _panelBg->setTextureRect(Rect(0, 0, 400, 350));
        _panelBg->setColor(Color3B(50, 50, 100));
        _panelBg->setOpacity(220);
    }
    _panelBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(_panelBg);

    // 标题
    auto title = Label::createWithSystemFont(u8"设置", "Arial", 36);
    title->setPosition(Vec2(_panelBg->getContentSize().width / 2,
        _panelBg->getContentSize().height - 40));
    _panelBg->addChild(title);

    // 退出游戏按钮
    Button* exitButton = nullptr;
	auto exitBtn = MenuItemImage::create(
        "exit_button.png", "exit_button.png",
		CC_CALLBACK_1(SettingLayer::onExitButtonClicked, this));
	exitBtn->setScale(0.1);
	exitBtn->setPosition(Vec2(_panelBg->getContentSize().width / 2, 150));

    // 关闭按钮
    Button* closeButton = nullptr;
    auto closeBtn = MenuItemImage::create(
        "out_of_now.png", "out_of_now.png",
        CC_CALLBACK_1(SettingLayer::onCloseButtonClicked, this));
	closeBtn->setPosition(Vec2(_panelBg->getContentSize().width - 50,
        _panelBg->getContentSize().height - 30));
	auto closeMenu = Menu::create(closeBtn,exitBtn, nullptr);
	closeMenu->setPosition(Vec2::ZERO);
	_panelBg->addChild(closeMenu,200);
}

void SettingLayer::createSimpleButton(Button*& button, const std::string& text, const Vec2& position, const std::function<void()>& callback)
{
    button = Button::create("ui/button_normal.png", "ui/button_pressed.png");
    if (!button) {
        button = Button::create();
        button->setContentSize(Size(120, 40));
        button->setTitleText(text);
        button->setTitleFontSize(20);
        button->setTitleColor(Color3B::WHITE);
        button->setColor(Color3B(80, 80, 160));
    }
    else {
        button->setTitleText(text);
        button->setTitleFontSize(20);
    }
    button->setPosition(position);
    button->addClickEventListener([callback](Ref* sender) {
        callback();
        });
    _panelBg->addChild(button,101);
}

void SettingLayer::onBackgroundMusicSliderChanged(Ref* sender, Slider::EventType type)
{
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
        _bgMusicVolume = _bgMusicSlider->getPercent() / 100.0f;
        setBackgroundMusicVolume(_bgMusicVolume);
    }
}

void SettingLayer::onSoundEffectSliderChanged(Ref* sender, Slider::EventType type)
{
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
        _soundEffectVolume = _soundEffectSlider->getPercent() / 100.0f;
        setSoundEffectVolume(_soundEffectVolume);

        // 测试音效
        playButtonClickSound();
    }
}

void SettingLayer::onExitButtonClicked(Ref* sender)
{
    playButtonClickSound();
    Director::getInstance()->end();
}

void SettingLayer::onCloseButtonClicked(Ref* sender)
{
    playButtonClickSound();
    hide();
}

void SettingLayer::show()
{
    this->setVisible(true);
}

void SettingLayer::hide()
{
    saveAudioSettings();
    if (_closeCallback) {
        _closeCallback();
    }
    this->setVisible(false);
}

void SettingLayer::loadAudioSettings()
{
    _bgMusicVolume = UserDefault::getInstance()->getFloatForKey("bg_music_volume", 0.5f);
    _soundEffectVolume = UserDefault::getInstance()->getFloatForKey("effect_volume", 0.8f);

    // 更新UI滑块
    if (_bgMusicSlider) {
        _bgMusicSlider->setPercent(_bgMusicVolume * 100);
    }
    if (_soundEffectSlider) {
        _soundEffectSlider->setPercent(_soundEffectVolume * 100);
    }

    // 设置音频音量
    setBackgroundMusicVolume(_bgMusicVolume);
    setSoundEffectVolume(_soundEffectVolume);
}

void SettingLayer::saveAudioSettings()
{
    UserDefault::getInstance()->setFloatForKey("bg_music_volume", _bgMusicVolume);
    UserDefault::getInstance()->setFloatForKey("effect_volume", _soundEffectVolume);
    UserDefault::getInstance()->flush();
}

void SettingLayer::playButtonClickSound()
{
    _audioEngine->playEffect("audio/button_click.mp3");
}

void SettingLayer::setBackgroundMusicVolume(float volume)
{
    _bgMusicVolume = volume;
    _audioEngine->setBackgroundMusicVolume(volume);
}

void SettingLayer::setSoundEffectVolume(float volume)
{
    _soundEffectVolume = volume;
    _audioEngine->setEffectsVolume(volume);
}