//struct { id, cost, level, ... }
#pragma once
#define MAX_LEVELS 5

enum class BuildingType
{
	TOWN_HALL = 0,
	CANNON = 1,
	ARCHER_TOWER = 2,
	MILITARY_CAMP = 3,//a mix of soldier housing and training
	ELIXIR_COLLECTOR = 4,
	GOLD_MINE = 5,
	WORKER_HOME = 6,
	GOLD_STORAGE = 7,
	ELIXIR_STORAGE = 8,
};

struct BuildingData 
{
	int id;// unique building identifier
	BuildingType type;
	int level;
	int maxBuildCount[MAX_LEVELS];// max number of this building type allowed at each level
	int goldCost[MAX_LEVELS];
	int elixirCost[MAX_LEVELS];
	int buildTime[MAX_LEVELS]; // in seconds
	int hitPoints[MAX_LEVELS]; // AKA health points
	int productionPerHour[MAX_LEVELS]; // for resource buildings
	int capacity[MAX_LEVELS]; // for storage buildings
	int attackRange[MAX_LEVELS]; // for defense buildings
	float damagePerSecond[MAX_LEVELS]; // for defense buildings
};
