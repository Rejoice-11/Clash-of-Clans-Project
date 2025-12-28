#include "HUDLayer.h"
#include "Classes/System/ResourceManager.h"

HUDLayer* HUDLayer::create()
{
    auto layer = new (std::nothrow) HUDLayer();

    if (layer && layer->init()) 
    {
        layer->autorelease();
        return layer;
    }

    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool HUDLayer::init()
{
    if (!Layer::init())
        return false;

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    // ========== 创建金币 UI ==========
    {
        // 图标（逻辑不变）
        _goldIcon = cocos2d::Sprite::create("coin.png");

        if (!_goldIcon) 
        {
            _goldIcon = cocos2d::Sprite::create();
            _goldIcon->setContentSize(cocos2d::Size(ICON_SIZE, ICON_SIZE));
            _goldIcon->setColor(cocos2d::Color3B(255, 215, 0));
        }

        float iconScale = ICON_SIZE / std::max(_goldIcon->getContentSize().width, 1.0f);
        _goldIcon->setScale(iconScale);

        // 文本（逻辑不变）
        _goldLabel = cocos2d::Label::createWithSystemFont("0/0", "Arial", 20);
        _goldLabel->setTextColor(cocos2d::Color4B(255, 255, 255, 255));
        _goldLabel->enableOutline(cocos2d::Color4B::BLACK, 1);

        // 背景条（逻辑不变）
        _goldBackgroundBar = cocos2d::Sprite::create("coin_bar_background.png");
        if (!_goldBackgroundBar) 
        {
            _goldBackgroundBar = cocos2d::Sprite::create();
            _goldBackgroundBar->setContentSize(cocos2d::Size(BAR_WIDTH, BAR_HEIGHT));
            _goldBackgroundBar->setColor(cocos2d::Color3B(60, 60, 60));
        }

        float barScaleX = BAR_WIDTH / std::max(_goldBackgroundBar->getContentSize().width, 1.0f);
        float barScaleY = BAR_HEIGHT / std::max(_goldBackgroundBar->getContentSize().height, 1.0f);
        _goldBackgroundBar->setScaleX(barScaleX);
        _goldBackgroundBar->setScaleY(barScaleY);

        // 填充条（逻辑不变）
        _goldFillBar = cocos2d::Sprite::create("coin_bar_full.png");
        if (!_goldFillBar) 
        {
            _goldFillBar = cocos2d::Sprite::create();
            _goldFillBar->setContentSize(cocos2d::Size(BAR_WIDTH, BAR_HEIGHT));
            _goldFillBar->setColor(cocos2d::Color3B(255, 215, 0));
        }

        _goldFillBar->setScaleX(barScaleX);
        _goldFillBar->setScaleY(barScaleY);
        _goldFillBar->setAnchorPoint(cocos2d::Vec2(1, 0.5f));
        _goldFillBar->setScaleX(0 * barScaleX);

        // 核心修改
        float iconPosY = ICON_SIZE / 2 - 5;
        _goldIcon->setPosition(cocos2d::Vec2(20, iconPosY)); 
        _goldLabel->setPosition(cocos2d::Vec2(ICON_SIZE + 100, iconPosY));
        _goldBackgroundBar->setPosition(cocos2d::Vec2(ICON_SIZE + 100, iconPosY));
        // 填充条位置随背景条自动右移
        _goldFillBar->setPosition(
            _goldBackgroundBar->getPositionX() + (_goldBackgroundBar->getContentSize().width * barScaleX) / 2,
            _goldBackgroundBar->getPositionY()
            );

        this->addChild(_goldIcon, 2);
        this->addChild(_goldLabel, 3);
        this->addChild(_goldBackgroundBar, 0);
        this->addChild(_goldFillBar, 1);
    }

    // ========== 创建圣水 UI ==========
    {
        // 图标（逻辑不变）
        _elixirIcon = cocos2d::Sprite::create("elixir.png");
        if (!_elixirIcon)
        {
            _elixirIcon = cocos2d::Sprite::create();
            _elixirIcon->setContentSize(cocos2d::Size(ICON_SIZE, ICON_SIZE));
            _elixirIcon->setColor(cocos2d::Color3B(186, 85, 211));
        }

        float iconScale = ICON_SIZE / std::max(_elixirIcon->getContentSize().width, 1.0f);
        _elixirIcon->setScale(iconScale);

        // 文本（逻辑不变）
        _elixirLabel = cocos2d::Label::createWithSystemFont("0/0", "Arial", 20);
        _elixirLabel->setTextColor(cocos2d::Color4B(255, 255, 255, 255));
        _elixirLabel->enableOutline(cocos2d::Color4B::BLACK, 1);

        // 背景条（逻辑不变）
        _elixirBackgroundBar = cocos2d::Sprite::create("elixir_bar_background.PNG");

        if (!_elixirBackgroundBar)
        {
            _elixirBackgroundBar = cocos2d::Sprite::create();
            _elixirBackgroundBar->setContentSize(cocos2d::Size(BAR_WIDTH, BAR_HEIGHT));
            _elixirBackgroundBar->setColor(cocos2d::Color3B(60, 60, 60));
        }

        float barScaleX = BAR_WIDTH / std::max(_elixirBackgroundBar->getContentSize().width, 1.0f);
        float barScaleY = BAR_HEIGHT / std::max(_elixirBackgroundBar->getContentSize().height, 1.0f);
        _elixirBackgroundBar->setScaleX(barScaleX);
        _elixirBackgroundBar->setScaleY(barScaleY);

        // 填充条（逻辑不变）
        _elixirFillBar = cocos2d::Sprite::create("elixir_bar_full.png");

        if (!_elixirFillBar)
        {
            _elixirFillBar = cocos2d::Sprite::create();
            _elixirFillBar->setContentSize(cocos2d::Size(BAR_WIDTH, BAR_HEIGHT));
            _elixirFillBar->setColor(cocos2d::Color3B(186, 85, 211));
        }

        _elixirFillBar->setScaleX(barScaleX);
        _elixirFillBar->setScaleY(barScaleY);
        _elixirFillBar->setAnchorPoint(cocos2d::Vec2(1, 0.5f));
        _elixirFillBar->setScaleX(0 * barScaleX);

        // 核心修改：圣水图标同步右移
        float offsetY = ICON_SIZE + 40;
        float iconPosY = ICON_SIZE / 2 - 5 + offsetY;
        _elixirIcon->setPosition(cocos2d::Vec2(20, iconPosY));
        _elixirLabel->setPosition(cocos2d::Vec2(ICON_SIZE + 100, iconPosY));
        _elixirBackgroundBar->setPosition(cocos2d::Vec2(ICON_SIZE + 100, iconPosY));
        // 填充条位置随背景条自动右移
        _elixirFillBar->setPosition(
            _elixirBackgroundBar->getPositionX() + (_elixirBackgroundBar->getContentSize().width * barScaleX) / 2,
            _elixirBackgroundBar->getPositionY()
            );

        this->addChild(_elixirIcon, 2);
        this->addChild(_elixirLabel, 3);
        this->addChild(_elixirBackgroundBar, 0);
        this->addChild(_elixirFillBar, 1);
    }

    // 整体Layer位置保持不变（如需更靠右，可微调此处，当前无需修改）
    this->setPosition(cocos2d::Vec2(
        origin.x + visibleSize.width - (ICON_SIZE + BAR_WIDTH + 30),
        origin.y + visibleSize.height - (ICON_SIZE + 40) - 50
        ));

    // 绑定回调（逻辑不变）
    auto rm = ResourceManager::getInstance();
    rm->setOnResourceChange([this]() 
    {
        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]() 
        {
        this->updateDisplay();
        });
    });

    updateDisplay(); // 初始刷新
    return true;
}

void HUDLayer::updateDisplay() 
{
    auto rm = ResourceManager::getInstance();

    // 更新金币（逻辑不变）
    int gold = rm->getGold();
    int maxGold = rm->getMaxGoldStorage();
    _goldLabel->setString(std::to_string(gold) + "/" + std::to_string(maxGold));
    float goldRatio = (maxGold > 0) ? static_cast<float>(gold) / maxGold : 0.0f;
    goldRatio = cocos2d::clampf(goldRatio, 0.0f, 1.0f);
    float goldBarBaseScaleX = BAR_WIDTH / std::max(_goldBackgroundBar->getContentSize().width, 1.0f);
    _goldFillBar->setScaleX(goldRatio * goldBarBaseScaleX);

    // 更新圣水（逻辑不变）
    int elixir = rm->getElixir();
    int maxElixir = rm->getMaxElixirStorage();
    _elixirLabel->setString(std::to_string(elixir) + "/" + std::to_string(maxElixir));
    float elixirRatio = (maxElixir > 0) ? static_cast<float>(elixir) / maxElixir : 0.0f;
    elixirRatio = cocos2d::clampf(elixirRatio, 0.0f, 1.0f);
    float elixirBarBaseScaleX = BAR_WIDTH / std::max(_elixirBackgroundBar->getContentSize().width, 1.0f);
    _elixirFillBar->setScaleX(elixirRatio * elixirBarBaseScaleX);
}