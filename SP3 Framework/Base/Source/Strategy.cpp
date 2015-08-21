#include "Strategy.h"
#include <iostream>

using namespace std;

CStrategy::CStrategy()
{
}

CStrategy::~CStrategy()
{
}

void CStrategy::Update(CMap* map, Vector3& enemyTile, Vector3& heroTile,Vector3& enemyDir, vector<CGoodies*> goodyList)
{
	cout << "void CStrategy::Update()" << endl;
}

int CStrategy::CalculateDistance()
{
	return ((theDestination.x - theEnemyPosition.x) * (theDestination.x - theEnemyPosition.x) +
			(theDestination.y - theEnemyPosition.y) * (theDestination.y - theEnemyPosition.y));
}
