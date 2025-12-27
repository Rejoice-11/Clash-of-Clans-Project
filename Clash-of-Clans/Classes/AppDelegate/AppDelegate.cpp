/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 http://www.cocos2d-x.org
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
 ****************************************************************************/

#include "Classes/AppDelegate/AppDelegate.h"
#include "Classes/Scene/MenuScene.h"
#include "Classes/Core/GameDirector.h"
#include "Classes/Scene/BattleScene.h"
#include "Classes/Core/ConfigManagerUnit.h"

 // 音频引擎（按需启用）
 // #define USE_AUDIO_ENGINE 1
 // #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

// >>>>>>>>>>>>>>>>>> 新增：Windows 平台头文件 <<<<<<<<<<<<<<<<<<
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>
#include "glfw3.h" // Cocos2d-x 内部已包含，但显式引用更安全
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

using namespace cocos2d;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

// >>>>>>>>>>>>>>>>>> 新增：Windows 消息处理 <<<<<<<<<<<<<<<<<<
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

// 全局保存原始窗口过程
static WNDPROC g_oldWndProc = nullptr;

// 自定义窗口过程：拦截 WM_CLOSE
LRESULT CALLBACK CustomWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_CLOSE) {
        // 安全退出，不直接关闭窗口
        cocos2d::Director::getInstance()->end();
        return 0; // 阻止默认行为
    }
    // 调用原始窗口过程
    return CallWindowProc(g_oldWndProc, hWnd, uMsg, wParam, lParam);
}

// 注册自定义窗口过程
void registerCloseHandler() {
    auto director = cocos2d::Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) return;

    // 获取 GLFW 窗口句柄 —— 使用 GLViewImpl 的 getWindow()
    auto glviewImpl = dynamic_cast<cocos2d::GLViewImpl*>(glview);
    if (!glviewImpl) return;

    GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(glviewImpl->getWindow());
    HWND hwnd = glfwGetWin32Window(glfwWindow);

    // 保存并替换窗口过程
    g_oldWndProc = reinterpret_cast<WNDPROC>(GetWindowLongPtr(hwnd, GWLP_WNDPROC));
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(CustomWndProc));
}

#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
    return 0;
}

bool AppDelegate::applicationDidFinishLaunching()
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("ClashOfTaffy", cocos2d::Rect(0, 0, 1280, 720));
#else
        glview = GLViewImpl::create("ClashOfTaffy");
#endif
        director->setOpenGLView(glview);
    }

    glview->setDesignResolutionSize(1280, 720, ResolutionPolicy::SHOW_ALL);

    // ===== Taffy 游戏初始化开始 =====
    GameDirector::getInstance()->init();
    GameDirector::getInstance()->runWithScene(MenuScene::createScene());

    // >>>>>>>>>>>>>>>>>> 新增：注册关闭处理器 <<<<<<<<<<<<<<<<<<
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    registerCloseHandler();
#endif
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
}