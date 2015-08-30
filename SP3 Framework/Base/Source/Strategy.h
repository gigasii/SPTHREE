#pragma once
#include <Vector2.h>
#include <Vector3.h>
#include "Pathing.h"
#include <vector>
#include "Goodies.h"
#include "GameObject.h"

class CStrategy
{

public:

	//The AI states of the enemy
	enum CURRENT_STATE
	{
		IDLE,
		PATROL,
		REPEL,
		ATTACK,
		NUM_ENEMY_STATE,
	};

	CStrategy();
	~CStrategy();

	virtual void Update(CMap* map, Vector3& enemyTile, Vector3& heroTile,Vector3& enemyDir, vector<CGoodies*> goodyList, int &routeCounter, int &routeCounter2, bool heroInvis, bool &eneHit, vector<GameObject*> &goList, float &eneROF, double dt) = 0;
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

	//Enemy AI State
	CStrategy::CURRENT_STATE CurrentState;

	bool isAttacking;
};

