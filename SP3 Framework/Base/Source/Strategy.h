#pragma once
#include <Vector2.h>
#include <Vector3.h>
#include "Pathing.h"

class CStrategy
{

public:
	CStrategy();
	~CStrategy();

	virtual void Update(CMap* map, Vector3& enemyTile, Vector3& heroTile) = 0;
	virtual void SetDestination(const float x, const float y) = 0;
	virtual void SetEnemyPosition(const float x, const float y) = 0;
	virtual void GetEnemyPosition(float& x, float& y) = 0;
	
	int CalculateDistance();

	//The Destination is the position of the target
	Vector2 theDestination;

	//The position of the enemy
	Vector2 theEnemyPosition;

	//The path of the enemy
	CPathing theEnemyPath;
};

