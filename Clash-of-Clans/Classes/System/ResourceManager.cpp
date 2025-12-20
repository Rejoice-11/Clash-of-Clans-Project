// Classes/System/ResourceManager.cpp
#include "ResourceManager.h"

ResourceManager* ResourceManager::s_instance = nullptr;

ResourceManager* ResourceManager::getInstance()
{
    if (!s_instance)
    {
        s_instance = new (std::nothrow) ResourceManager();
    }
    return s_instance;
}

void ResourceManager::destroyInstance()
{
    CC_SAFE_DELETE(s_instance);
}

ResourceManager::ResourceManager()
{
    // 默认初始资源，可根据需要从存档或配置加载
    _gold = 1000;
    _elixir = 500;
}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::canAfford(int gold, int elixir) const
{
    return (_gold >= gold) && (_elixir >= elixir);
}

bool ResourceManager::spend(int gold, int elixir)
{
    if (!canAfford(gold, elixir)) return false;
    _gold -= gold;
    _elixir -= elixir;
    return true;
}

void ResourceManager::addResources(int gold, int elixir)
{
    _gold += gold;
    _elixir += elixir;
}