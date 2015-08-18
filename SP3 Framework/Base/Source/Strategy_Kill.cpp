#include "Strategy_Kill.h"

CStrategy_Kill::CStrategy_Kill()
{
}

CStrategy_Kill::~CStrategy_Kill()
{
}

void CStrategy_Kill::Update()
{
	//Decide which state to change to
	int distanceHeroToEnemy = CalculateDistance();
	
	if(distanceHeroToEnemy < 10000.0f)
	{
		if(distanceHeroToEnemy < 625.0f)
		{
			CurrentState = REPEL;
		}

		else
		{
			CurrentState = ATTACK;
		}
	}

	else
	{
		if (CurrentState != PATROL)
			theEnemyPath.location = 0;

		CurrentState = PATROL;
	}

	//Based on the current state, move the enemy
	switch(CurrentState)
	{
		case ATTACK:
			theEnemyPosition.x = theEnemyPosition.x + (theDestination.x - theEnemyPosition.x > 0 ? 2 : - 2);
			theEnemyPosition.y = theEnemyPosition.y + (theDestination.y - theEnemyPosition.y > 0 ? 2 : - 2);
			break;
		
		case REPEL:
			theEnemyPosition.x = theEnemyPosition.x + (theDestination.x - theEnemyPosition.x <= 0 ? 2 : - 2);
			theEnemyPosition.y = theEnemyPosition.y + (theDestination.y - theEnemyPosition.y <= 0 ? 2 : - 2);
			break;
		
		case PATROL:
			theEnemyPath.executePath(theEnemyPosition.x,theEnemyPosition.y,2);
			break;
	}
}

void CStrategy_Kill::SetDestination(const float x, const float y)
{
	theDestination.x = x;
	theDestination.y = y;
}

void CStrategy_Kill::SetEnemyPosition(const float x, const float y)
{
	theEnemyPosition.x = x;
	theEnemyPosition.y = y;
}

void CStrategy_Kill::GetEnemyPosition(float& x, float& y)
{
	x = theEnemyPosition.x;
	y = theEnemyPosition.y;
}

void CStrategy_Kill::SetState(CStrategy_Kill::CURRENT_STATE theEnemyState)
{
	CurrentState = theEnemyState;
}

CStrategy_Kill::CURRENT_STATE CStrategy_Kill::GetState()
{
	return CurrentState;
}

