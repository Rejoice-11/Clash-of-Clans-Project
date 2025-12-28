// WorkerHome.h
#pragma once
#include "Classes/Entity/Building/Building.h"
#include "Classes/Data/BuildingData.h"

extern BuildingData WorkerHomeBuildingData; // 外部数据声明

/**
 * @brief 工人小屋 - 提供建造队列容量
 * @note 每级提供固定工人数量（即建造队列槽位）
 */
class WorkerHome : public Building {
public:
    explicit WorkerHome(const BuildingData& data, int instanceId = -1);

    bool isDefenseStructure() const override { return false; }
    void upgrade() override;

    /** @brief 获取当前等级提供的工人数量（建造队列容量） */
    int getWorkerCount() const;

    cocos2d::Sprite* createSprite() override;

private:
    std::string getSpriteFrameName() const;
};