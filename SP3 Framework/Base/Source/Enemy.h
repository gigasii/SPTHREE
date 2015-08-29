#pragma once
#include <Vector2.h>
#include "Strategy_Kill.h"
#include "Pathing.h"
#include <Vector3.h>

class CEnemy
{

private:
	//Enemy's information
	bool enemyAnimationInvert;
	bool enemyAnimationFlip;

	//The Destination is the position of the Hero
	Vector2 theDestination;

public:
	CEnemy();
	~CEnemy();

	enum STRATEGY_TYPE
	{
		STRAT_KILL,
		STRAT_SAFE,
		STRAT_NULL,
		NUM_STRATEGY,
	};

	STRATEGY_TYPE currentStrat;
	CStrategy *theStrategy;
	Vector3 theENEMYPosition;

	int ID;
	int health;
	bool active;
	CPathing path;
	Vector3 eneCurrTile;
	Vector3 direction;
	float enemyTileID;
	float stunTileID;
	bool attackStatus;
	float attackReactionTime;
	bool attackAnimation;
	float attackAnimationTimer;
	vector <Vector3>detectionGrid;
	bool stunned;
	double stunTimer;
	bool isHit;
	int routeCounter, routeCounter2;

	//Initialise this class instance
	void Init();

	//Set position x of the enemy
	void SetPos_x(int pos_x);
	//Get position x of the enemy
	int GetPos_x();

	//Set position y of the enemy
	void SetPos_y(int pos_y);
	//Get position y of the enemy
	int GetPos_y();

	//Set the destination of this enemy
	void SetDestination(const int pos_x, const int pos_y);
	//Get destination of this enemy
	int GetDestination_x();
	//Get destination of this enemy
	int GetDestination_y();

	//Get Animation Invert status of the enemy
	bool GetAnimationInvert();

	//Get Animation Flip Status of the enemy
	bool GetAnimationFlip();

	//Enemy Update
	void Update(CMap* map, Vector3& heroTile, vector<CGoodies*> goodyList, bool heroInvis);

	//Strategy
	void ChangeStrategy(CStrategy* theNewStrategy, bool bDelete = true);
	void setWayPoints(CMap* map);
};

