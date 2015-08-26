#include "Strategy.h"
#include <iostream>

using namespace std;

CStrategy::CStrategy() : isAttacking(false)
{
}

CStrategy::~CStrategy()
{
}

void CStrategy::Update(CMap* map, Vector3& enemyTile, Vector3& heroTile,Vector3& enemyDir, vector<CGoodies*> goodyList, int &routeCounter, int &routeCounter2)
{
	cout << "void CStrategy::Update()" << endl;
}

int CStrategy::CalculateDistance()
{
	return ((theDestination.x - theEnemyPosition.x) * (theDestination.x - theEnemyPosition.x) +
			(theDestination.y - theEnemyPosition.y) * (theDestination.y - theEnemyPosition.y));
}
