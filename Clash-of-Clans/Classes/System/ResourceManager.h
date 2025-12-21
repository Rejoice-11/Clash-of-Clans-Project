//金币/圣水增减、上限校验
#pragma once

#include <functional>
#include <mutex>

/**
 * @brief 全局资源管理器（金币、圣水）
 * @note 单例模式，负责资源增减与存储上限校验
 */
class ResourceManager 
{
public:
    // ———————— 单例访问 ———————— //
    static ResourceManager* getInstance();
    static void destroyInstance();

    // ———————— 资源操作 ———————— //
    /**
     * @brief 尝试增加金币
     * @param amount 增加量（必须 >0）
     * @return true 成功（未超上限），false 失败（储罐已满）
     */
    bool addGold(int amount);
    bool addElixir(int amount);

    /**
     * @brief 消耗资源（建造/升级用）
     * @return true 成功（资源足够），false 失败
     */
    bool spendGold(int amount);
    bool spendElixir(int amount);

    // ———————— 只读查询 ———————— //
    int getGold() const { return _gold; }
    int getElixir() const { return _elixir; }

    // 存储上限由外部系统（如储金罐等级）决定
    void setMaxGoldStorage(int max) { _maxGoldStorage = max; }
    void setMaxElixirStorage(int max) { _maxElixirStorage = max; }
    int getMaxGoldStorage() const { return _maxGoldStorage; }
    int getMaxElixirStorage() const { return _maxElixirStorage; }

    // ———————— 事件回调（用于刷新UI） ———————— //
    using ResourceChangeListener = std::function<void()>;
    void setOnResourceChange(const ResourceChangeListener& listener) {
        _onResourceChange = listener;
    }

private:
    ResourceManager();
    ~ResourceManager() = default;

    // 禁止拷贝
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    void notifyResourceChange();

    static ResourceManager* _instance;
    mutable std::mutex _mutex;

    int _gold = 5000;           // 初始值可配置
    int _elixir = 5000;
    int _maxGoldStorage = 10000;
    int _maxElixirStorage = 10000;

    ResourceChangeListener _onResourceChange;
};