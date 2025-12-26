#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

class SettingLayer : public Layer
{
public:
    CREATE_FUNC(SettingLayer);

    virtual bool init();

    // 设置回调函数
    void setExitCallback(const std::function<void()>& callback) { _exitCallback = callback; }
    void setCloseCallback(const std::function<void()>& callback) { _closeCallback = callback; }

    // 显示/隐藏设置层
    void show();
    void hide();
    bool isVisible() const { return Layer::isVisible(); }

    // 音频相关功能
    void loadAudioSettings();
    void saveAudioSettings();
    void playButtonClickSound();
    void setBackgroundMusicVolume(float volume);
    void setSoundEffectVolume(float volume);
    float getBackgroundMusicVolume() const { return _bgMusicVolume; }
    float getSoundEffectVolume() const { return _soundEffectVolume; }

private:
    void initUI();
    void onBackgroundMusicSliderChanged(Ref* sender, Slider::EventType type);
    void onSoundEffectSliderChanged(Ref* sender, Slider::EventType type);
    void onExitButtonClicked(Ref* sender);
    void onCloseButtonClicked(Ref* sender);
    void createSimpleButton(Button*& button, const std::string& text, const Vec2& position, const std::function<void()>& callback);

    // UI元素
    Slider* _bgMusicSlider;
    Slider* _soundEffectSlider;
    Sprite* _panelBg;

    // 音量值
    float _bgMusicVolume;
    float _soundEffectVolume;

    // 回调函数
    std::function<void()> _exitCallback;
    std::function<void()> _closeCallback;

    // 音频实例
    SimpleAudioEngine* _audioEngine;
};