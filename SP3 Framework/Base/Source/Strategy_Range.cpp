#include "Strategy_Range.h"

CStrategy_Range::CStrategy_Range()
{
}

CStrategy_Range::~CStrategy_Range()
{
}

void CStrategy_Range::Update(CMap* map, Vector3& enemyTile, Vector3& heroTile, Vector3& enemyDir, vector<CGoodies*> goodyList, int &routeCounter, int &routeCounter2, bool heroInvis, bool &eneHit, vector<GameObject*> &goList, float &eneROF, double dt)
{
	//Decide which state to change to
	int distanceHeroToEnemy = CalculateDistance();

	if ((distanceHeroToEnemy <= 200000.0f || eneHit == true) && !(map->theScreenMap[heroTile.y][heroTile.x] == CMap::HAY || heroInvis == true))
	{
		if (CurrentState != ATTACK)
			CurrentState = ATTACK;
	}
	else
	{
		if (CurrentState != IDLE)
			CurrentState = IDLE;
	}

	switch (CurrentState)
	{
	case ATTACK:
		{
			eneROF += dt;

			if (eneROF >= 1)
			{
				eneROF = 0;

				GameObject* go = new GameObject(GameObject::GO_BULLET);
				goList.push_back(go);
				go->active = true;
				go->scale.Set(6,6,6);
				go->pos.Set(enemyTile.x * map->GetTileSize() + 16, map->GetTileSize() * (map->GetNumOfTiles_Height() - enemyTile.y) -  map->GetTileSize() + 16);
				Vector3 direction = (heroTile - enemyTile).Normalize();
				direction.y = -direction.y;
				go->vel = direction * 500;

				if (eneHit == true)
					eneHit = false;
			}
		}
		break;

	case IDLE:
		break;
	}

}

void CStrategy_Range::SetDestination(const float x, const float y)
{
	theDestination.x = x;
	theDestination.y = y;
}

void CStrategy_Range::SetEnemyPosition(const float x, const float y)
{
	theEnemyPosition.x = x;
	theEnemyPosition.y = y;
}

void CStrategy_Range::GetEnemyPosition(float& x, float& y)
{
	x = theEnemyPosition.x;
	y = theEnemyPosition.y;
}