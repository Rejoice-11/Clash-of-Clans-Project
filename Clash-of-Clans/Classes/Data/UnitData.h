//struct { hp, damage, range, ... }
#pragma once

enum class UnitType 
{
	MELEE = 0,
	RANGED = 1,
	TANK = 2,
	WALL_BREAKER = 3,
	// more unit types can be added here
};

struct UnitData 
{
	int id;
	UnitType type;//unit type identifier
	int costForHavingOne;//in people capacity
	int hp;
	int damage;
	float attackRange; // in pixels
	float attackInterval; // ÀıÈç£ºÒ°ÂùÈË 1.0s, ¾ŞÈË 2.0s
	float speed;       // in pixels per second
};