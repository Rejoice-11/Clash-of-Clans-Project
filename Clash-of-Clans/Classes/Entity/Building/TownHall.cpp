#include "Classes/Entity/Building/TownHall.h"
#include "Classes/Data/BuildingData.h"
#include "cocos2d.h"
USING_NS_CC;
// 构造函数实现
TownHall::TownHall(const BuildingData& data, int instanceId)
	: Building(data, instanceId) 
{
	countofTownHallsInVillage++;
}
// 创建渲染精灵实现
Sprite* TownHall::createSprite() 
{
	std::string frameName = getSpriteFrameName();
	auto sprite = Sprite::create(frameName);
	return sprite;
}
// 升级逻辑实现
void TownHall::upgrade() 
{
	if (_currentLevel < MAX_LEVELS) { // 假设大本营最高等级为3
		_currentLevel++;
		// 这里可以添加更多升级逻辑，如提升HP、改变外观等
	}
}

// 获取当前等级对应的精灵名称实现
std::string TownHall::getSpriteFrameName() const 
{
	return "town_hall_lv" + std::to_string(_currentLevel + 1) + ".png";
}

int getCountTownHallsInVillage() {
	return countofTownHallsInVillage;
}
