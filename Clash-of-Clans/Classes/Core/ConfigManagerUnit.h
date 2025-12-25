//加载JSON配置（兵种数据）
#pragma once
#include <map>
#include <string>
#include "Classes/Data/UnitData.h"

class ConfigManager {
public:
    static ConfigManager* getInstance();

    // 启动时调用：加载 JSON 文件
    bool loadConfigs(const std::string& filename);

    // 给工厂用的接口：根据 ID 获取对应的数值图纸
    const UnitData& getUnitData(int id);

private:
    ConfigManager() = default;
    static ConfigManager* _instance;

    // 内存中的图纸库：ID -> 数值结构体
    std::map<int, UnitData> _unitConfigs;

    // 一个默认值，防止查询失败崩溃
    UnitData _defaultData;
};