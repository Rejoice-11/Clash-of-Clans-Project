// Classes/System/ResourceManager.cpp
#include "Classes/System/ResourceManager.h"
#include "cocos2d.h"

// 静态成员定义
ResourceManager* ResourceManager::_instance = nullptr;

void ResourceManager::syncBuildingCounts(
    const std::map<StoreWindow::BuildingType, int>& counts) 
{
    _buildingCounts = counts; // 全量更新
}

int ResourceManager::getBuildingCount(StoreWindow::BuildingType type) const
{
    auto it = _buildingCounts.find(type);
    return (it != _buildingCounts.end()) ? it->second : 0;
}

// ———————— 单例实现 ———————— //

ResourceManager* ResourceManager::getInstance() 
{
    if (!_instance) 
    {
        _instance = new ResourceManager();
    }
    return _instance;
}

void ResourceManager::destroyInstance() 
{
    delete _instance;
    _instance = nullptr;
}

ResourceManager::ResourceManager() 
{
    // 可从存档加载初始值
    cocos2d::log("ResourceManager initialized: Gold=%d, Elixir=%d", _gold, _elixir);
}

// ———————— 资源操作实现 ———————— //

bool ResourceManager::addGold(int amount) 
{
    if (amount <= 0) 
        return false;

    std::lock_guard<std::mutex> lock(_mutex);

    if (_gold + amount > _maxGoldStorage) 
    {
		_gold = _maxGoldStorage;
        notifyResourceChange();
        return false; // 超出上限
    }

    _gold += amount;
    notifyResourceChange();
    return true;
}

bool ResourceManager::addElixir(int amount) 
{
    if (amount <= 0) 
        return false;

    std::lock_guard<std::mutex> lock(_mutex);

    if (_elixir + amount > _maxElixirStorage) 
    {
		_elixir = _maxElixirStorage;
        notifyResourceChange();
        return false;
    }

    _elixir += amount;
    notifyResourceChange();
    return true;
}

bool ResourceManager::spendGold(int amount) 
{
    if (amount <= 0) 
        return false;

    std::lock_guard<std::mutex> lock(_mutex);

    if (_gold < amount) 
    {
        return false; // 资源不足
    }

    _gold -= amount;
    notifyResourceChange();

    return true;
}

bool ResourceManager::spendElixir(int amount) 
{
    if (amount <= 0)
        return false;

    std::lock_guard<std::mutex> lock(_mutex);

    if (_elixir < amount) 
    {
        return false;
    }

    _elixir -= amount;
    notifyResourceChange();

    return true;
}

void ResourceManager::updateMaxGoldStorage(int totalCapacity)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _maxGoldStorage = totalCapacity;
    // 自动裁剪超限资源（可选）

    if (_gold > _maxGoldStorage) 
        _gold = _maxGoldStorage;

    notifyResourceChange(); // ← 关键！触发 HUD 刷新
}

void ResourceManager::updateMaxElixirStorage(int totalCapacity)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _maxElixirStorage = totalCapacity;

    if (_elixir > _maxElixirStorage)
        _elixir = _maxElixirStorage;

    notifyResourceChange();
}

// ———————— 事件通知 ———————— //

void ResourceManager::notifyResourceChange() 
{
    if (_onResourceChange) 
    {
        _onResourceChange();
    }
}