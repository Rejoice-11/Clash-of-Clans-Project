#include "HUDLayer.h"

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
    if (!Layer::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 创建标签（右上角）
    _goldLabel = Label::createWithSystemFont("", "Arial", 24);
    _goldLabel->setTextColor(Color4B(255, 215, 0, 255)); // 金色
    _goldLabel->setPosition(Vec2(origin.x + visibleSize.width - 100, origin.y + visibleSize.height - 30));
    this->addChild(_goldLabel);

    _elixirLabel = Label::createWithSystemFont("", "Arial", 24);
    _elixirLabel->setTextColor(Color4B(186, 85, 211, 255)); // 紫色
    _elixirLabel->setPosition(Vec2(origin.x + visibleSize.width - 100, origin.y + visibleSize.height - 60));
    this->addChild(_elixirLabel);

    // 绑定 ResourceManager 回调
    ResourceManager::getInstance()->setOnResourceChange([this]() {
        this->onResourceChanged();
        });

    // 初始刷新
    updateResourceLabels();

    return true;
}

void HUDLayer::onResourceChanged() 
{
    // 注意：回调可能在子线程触发！需回到主线程更新 UI
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]()
        {
        this->updateResourceLabels();
        });
}

void HUDLayer::updateResourceLabels()
{
    auto rm = ResourceManager::getInstance();
    int gold = rm->getGold();
    int maxGold = rm->getMaxGoldStorage();
    int elixir = rm->getElixir();
    int maxElixir = rm->getMaxElixirStorage();

    _goldLabel->setString(std::to_string(gold) + "/" + std::to_string(maxGold));
    _elixirLabel->setString(std::to_string(elixir) + "/" + std::to_string(maxElixir));
}