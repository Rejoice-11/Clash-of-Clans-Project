#include "Classes/Entity/Building/ResourceBuilding.h"
#include "Classes/System/ResourceManager.h"
#include "cocos2d.h"

// 构造函数
ResourceBuilding::ResourceBuilding(const BuildingData& data, int instanceId, ResourceType type)
    : Building(data, instanceId)
    , _resourceType(type)
    , _accumulatedTime(0.0f) 
{
    // 可在此初始化其他成员
}

// ———————— 重写基类虚函数 ————————

cocos2d::Sprite* ResourceBuilding::createSprite() 
{
    std::string frameName = getSpriteFrameName();
    auto sprite = cocos2d::Sprite::create(frameName + ".png");
    if (!sprite) 
    {
        // 若找不到具体等级图，回退到通用图
        std::string fallback = (_resourceType == ResourceType::GOLD) ? "gold_mine_lv1.png" : "elixir_collector_lv1.png";
        sprite = cocos2d::Sprite::create(fallback);
    }
    return sprite;
}

void ResourceBuilding::upgrade() 
{
    if (_currentLevel < MAX_LEVELS) 
    {
        _currentLevel++;
        // 可在此触发UI更新、播放音效等
    }
}

std::string ResourceBuilding::getSpriteFrameName() const 
{
    std::string baseName = (_resourceType == ResourceType::GOLD) ? "gold_mine_lv" : "elixir_collector_lv";
    return baseName + std::to_string(_currentLevel + 1);
}

// ———————— 资源产出逻辑 ————————

int ResourceBuilding::produce(float deltaTime) 
{
    // 安全校验：等级有效？
    if (_currentLevel < 0 || _currentLevel >= MAX_LEVELS )
    {
        return 0;
    }

    // 注意：你的 BuildingData 必须包含 productionPerHour 字段！
    // 如果还没有，请在 BuildingLevelData 中添加：
    //   int productionPerHour;
    float productionRate = static_cast<float>(_data.productionPerHour[_currentLevel]) / 3600.0f; // 转为每秒产量

    _accumulatedTime += deltaTime;
    int produced = 0;

    // 每积累1秒就尝试产出一次（避免浮点误差累积）
    while (_accumulatedTime >= 1.0f) 
    {
        _accumulatedTime -= 1.0f;

        bool success = false;

        if (_resourceType == ResourceType::GOLD) 
        {
            success = ResourceManager::getInstance()->addGold(int(productionRate));
        }

        else 
        {
            success = ResourceManager::getInstance()->addElixir(int(productionRate));
        }

        if (success) 
        {
            produced++;
        }

        else 
        {
            // 储罐已满，停止本次产出循环
            break;
        }
    }

    return produced;
}