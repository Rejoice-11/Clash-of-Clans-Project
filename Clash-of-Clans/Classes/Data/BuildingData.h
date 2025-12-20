//struct { id, cost, level, ... }
#pragma once
#include <vector>

enum class BuildingType {
	TOWN_HALL = 0,
	CANNON = 1,
	ARCHER_TOWER = 2,
	MINITARY_CAMP = 3,
	ELIXIR_COLLECTOR = 4,
	GOLD_MINE = 5,
	WORKER_HOME = 6,


};
struct BuildingLevelData {
	int id;
	int goldCost;
	int elixirCost;
	int buildTime; // in seconds
	int hitPoints;
	// Add other level-specific attributes as needed
};
