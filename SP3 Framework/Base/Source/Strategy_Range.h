#pragma once
#include <Vector2.h>
#include "Strategy.h"


class CMap;

class CStrategy_Range : public CStrategy
{

public:
	CStrategy_Range();
	~CStrategy_Range();

	void Update(CMap* map, Vector3& enemyTile, Vector3& heroTile, Vector3& enemyDir, vector<CGoodies*> goodyList, int &routeCounter, int &routeCounter2, bool heroInvis, bool &eneHit, vector<GameObject*> &goList, float &eneROF, double dt);
	void SetDestination(const float x, const float y);
	void SetEnemyPosition(const float x, const float y);
	void GetEnemyPosition(float& x, float& y);
};

