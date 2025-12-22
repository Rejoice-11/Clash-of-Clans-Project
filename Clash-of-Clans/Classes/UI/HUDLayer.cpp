#include "HUDLayer.h"
#include "Classes/System/ResourceManager.h"

HUDLayer* HUDLayer::create() {
    auto layer = new (std::nothrow) HUDLayer();
    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool HUDLayer::init() 
{
    if (!Layer::init()) return false;

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    // ========== 创建金币 UI ==========
    {
        // 图标
        _goldIcon = cocos2d::Sprite::create("coin.png");
        if (!_goldIcon) {
            _goldIcon = cocos2d::Sprite::create();
            _goldIcon->setContentSize(cocos2d::Size(ICON_SIZE, ICON_SIZE));
            _goldIcon->setColor(cocos2d::Color3B(255, 215, 0));
        }
        float scale = ICON_SIZE / std::max(_goldIcon->getContentSize().width, 1.0f);
        _goldIcon->setScale(scale);

        // 文本
        _goldLabel = cocos2d::Label::createWithSystemFont("0/0", "Arial", 18);
        _goldLabel->setTextColor(cocos2d::Color4B(255, 255, 255, 255));
        _goldLabel->enableOutline(cocos2d::Color4B::BLACK, 1);

        // 背景条（灰色）
        _goldBackgroundBar = cocos2d::Sprite::create("coin_bar_background.png");
        if (!_goldBackgroundBar) {
            _goldBackgroundBar = cocos2d::Sprite::create();
            _goldBackgroundBar->setContentSize(cocos2d::Size(BAR_WIDTH, BAR_HEIGHT));
            _goldBackgroundBar->setColor(cocos2d::Color3B(60, 60, 60));
        }

        // 填充条（黄色）→ 从右往左覆盖
        _goldFillBar = cocos2d::Sprite::create("coin_bar_full.png");
        if (!_goldFillBar) {
            _goldFillBar = cocos2d::Sprite::create();
            _goldFillBar->setContentSize(cocos2d::Size(BAR_WIDTH, BAR_HEIGHT));
            _goldFillBar->setColor(cocos2d::Color3B(255, 215, 0));
        }
        _goldFillBar->setAnchorPoint(cocos2d::Vec2(1, 0.5f)); // ← 关键！从右侧开始
        _goldFillBar->setScaleX(0); // 初始为0

        // 布局：金币在上
        float barY = ICON_SIZE / 2 - 10;
        _goldIcon->setPosition(cocos2d::Vec2(-125, ICON_SIZE / 2 - 10));
        _goldLabel->setPosition(cocos2d::Vec2(ICON_SIZE + 10, ICON_SIZE / 2 - 10));
        _goldBackgroundBar->setPosition(cocos2d::Vec2(ICON_SIZE + 10, ICON_SIZE / 2 - 10));
        _goldFillBar->setPosition(_goldBackgroundBar->getPosition());

        this->addChild(_goldIcon, 2);
        this->addChild(_goldLabel, 3);
        this->addChild(_goldBackgroundBar, 0);
        this->addChild(_goldFillBar, 1);
    }

    // ========== 创建圣水 UI ==========
    {
        // 图标
        _elixirIcon = cocos2d::Sprite::create("elixir.png");
        if (!_elixirIcon) {
            _elixirIcon = cocos2d::Sprite::create();
            _elixirIcon->setContentSize(cocos2d::Size(ICON_SIZE, ICON_SIZE));
            _elixirIcon->setColor(cocos2d::Color3B(186, 85, 211));
        }
        float scale = ICON_SIZE / std::max(_elixirIcon->getContentSize().width, 1.0f);
        _elixirIcon->setScale(scale);

        // 文本
        _elixirLabel = cocos2d::Label::createWithSystemFont("0/0", "Arial", 18);
        _elixirLabel->setTextColor(cocos2d::Color4B(255, 255, 255, 255));
        _elixirLabel->enableOutline(cocos2d::Color4B::BLACK, 1);

        // 背景条（紫色）
        _elixirBackgroundBar = cocos2d::Sprite::create("elixir_bar_background.PNG");
        if (!_elixirBackgroundBar) {
            _elixirBackgroundBar = cocos2d::Sprite::create();
            _elixirBackgroundBar->setContentSize(cocos2d::Size(BAR_WIDTH, BAR_HEIGHT));
            _elixirBackgroundBar->setColor(cocos2d::Color3B(60, 60, 60));
        }

        // 填充条（紫色）→ 从右往左覆盖
        _elixirFillBar = cocos2d::Sprite::create("elixir_bar_full.png");
        if (!_elixirFillBar) {
            _elixirFillBar = cocos2d::Sprite::create();
            _elixirFillBar->setContentSize(cocos2d::Size(BAR_WIDTH, BAR_HEIGHT));
            _elixirFillBar->setColor(cocos2d::Color3B(186, 85, 211));
        }
        _elixirFillBar->setAnchorPoint(cocos2d::Vec2(1, 0.5f)); // ← 关键！从右侧开始
        _elixirFillBar->setScaleX(0); // 初始为0

        // 布局：圣水在下
        float offsetY = ICON_SIZE + 30;
        float barY = ICON_SIZE / 2 - 10 + offsetY;
        _elixirIcon->setPosition(cocos2d::Vec2(-125, barY));
        _elixirLabel->setPosition(cocos2d::Vec2(ICON_SIZE + 10, barY));
        _elixirBackgroundBar->setPosition(cocos2d::Vec2(ICON_SIZE + 10, barY));
        _elixirFillBar->setPosition(_elixirBackgroundBar->getPosition());

        this->addChild(_elixirIcon, 2);
        this->addChild(_elixirLabel, 3);
        this->addChild(_elixirBackgroundBar, 0);
        this->addChild(_elixirFillBar, 1);
    }

    // 设置整体位置（右上角）
    this->setPosition(cocos2d::Vec2(
        origin.x + visibleSize.width - (ICON_SIZE + BAR_WIDTH + 30),
        origin.y + visibleSize.height - (ICON_SIZE + 30) - 50
    ));

    // 绑定 ResourceManager 回调
    auto rm = ResourceManager::getInstance();
    rm->setOnResourceChange([this]() {
        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]() {
            this->updateDisplay();
            });
        });

    updateDisplay(); // 初始刷新
    return true;
}

void HUDLayer::updateDisplay() {
    auto rm = ResourceManager::getInstance();

    // 更新金币
    int gold = rm->getGold();
    int maxGold = rm->getMaxGoldStorage();
    _goldLabel->setString(std::to_string(gold) + "/" + std::to_string(maxGold));
    float goldRatio = (maxGold > 0) ? static_cast<float>(gold) / maxGold : 0.0f;
    _goldFillBar->setScaleX(cocos2d::clampf(goldRatio, 0.0f, 1.0f));

    // 更新圣水
    int elixir = rm->getElixir();
    int maxElixir = rm->getMaxElixirStorage();
    _elixirLabel->setString(std::to_string(elixir) + "/" + std::to_string(maxElixir));
    float elixirRatio = (maxElixir > 0) ? static_cast<float>(elixir) / maxElixir : 0.0f;
    _elixirFillBar->setScaleX(cocos2d::clampf(elixirRatio, 0.0f, 1.0f));
}