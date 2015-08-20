#include "Enemy.h"
#include <iostream>
#include "Map.h"

CEnemy::CEnemy()
	: theENEMYPosition(0, 0)
	, enemyAnimationCounter(0)
	, enemyAnimationInvert(false)
	, theStrategy(NULL)
	, active(true)
	, ID(50)
	, eneCurrTile(0,0,0)
{
}

CEnemy::~CEnemy()
{
	if(theStrategy != NULL)
	{
		delete theStrategy;
		theStrategy = NULL;
	}
}

//Initialise this class instance
void CEnemy::Init()
{
	theENEMYPosition.x = 0;
	theENEMYPosition.y = 0;
}

//Set position x of the enemy
void CEnemy::SetPos_x(int pos_x)
{
	theENEMYPosition.x = pos_x;
}

//Set position y of the enemy
void CEnemy::SetPos_y(int pos_y)
{
	theENEMYPosition.y = pos_y;
}

//Set the destination of this enemy
void CEnemy::SetDestination(const int pos_x, const int pos_y)
{
	theDestination.x = pos_x;
	theDestination.y = pos_y;

	if(theStrategy != NULL)
	{
		theStrategy->SetDestination(theDestination.x, theDestination.y);
	}
}

//Set Animation Invert status of the player
void CEnemy::SetAnimationInvert(bool enemyAnimationInvert)
{
	this->enemyAnimationInvert = enemyAnimationInvert;
}

//Set Animation Counter of the player
void CEnemy::SetAnimationCounter(int enemyAnimationCounter)
{
	this->enemyAnimationCounter = enemyAnimationCounter;
}

//Get position x of the enemy
int CEnemy::GetPos_x()
{
	return theENEMYPosition.x;
}

//Get position y of the enemy
int CEnemy::GetPos_y()
{
	return theENEMYPosition.y;
}

//Get the destination x of this enemey
int CEnemy::GetDestination_x()
{
	return theDestination.x;
}

//Get the destination y of this enemy
int CEnemy::GetDestination_y()
{
	return theDestination.y;
}

//Get Animation Invert status of thep layer
bool CEnemy::GetAnimationInvert()
{
	return enemyAnimationInvert;
}

//Get Animation Counter of the player
int CEnemy::GetAnimationCounter()
{
	return enemyAnimationCounter;
}

//Enemy Update
void CEnemy::Update(CMap* map, Vector3& heroTile)
{
	if(theStrategy != NULL)
	{
		theStrategy->Update(map, eneCurrTile, heroTile);
		theStrategy->GetEnemyPosition((theENEMYPosition.x), (theENEMYPosition.y));
	}
}

//Strategy
void CEnemy::ChangeStrategy(CStrategy* theNewStrategy, bool bDelete)
{
	if(bDelete == true)
	{
		if(theStrategy != NULL)
		{
			delete theStrategy;
			theStrategy = NULL;
		}
	}

	theStrategy = theNewStrategy;
	if(theStrategy != NULL)
	{
		theStrategy->SetDestination(theDestination.x, theDestination.y);
		theStrategy->SetEnemyPosition(theENEMYPosition.x, theENEMYPosition.y);
		theStrategy->theEnemyPath = path;
	}
}

void CEnemy::setWayPoints(CMap* map)
{
	switch(ID)
	{
		case CMap::ENEMY_1:
		path.setWayPoints(map, 4, CMap::WAYPOINT_1, CMap::WAYPOINT_2, CMap::WAYPOINT_3, CMap::WAYPOINT_4);
		break;
	}
}