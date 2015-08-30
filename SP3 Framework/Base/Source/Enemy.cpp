#include "Enemy.h"
#include <iostream>
#include "Map.h"

CEnemy::CEnemy()
	: theENEMYPosition(0, 0)
	, enemyAnimationInvert(false)
	, enemyAnimationFlip(false)
	, theStrategy(NULL)
	, active(true)
	, ID(0)
	, health(0)
	, eneCurrTile(0, 0, 0)
	, direction(0, 0, 0)
	, enemyTileID(0)
	, stunTileID(0)
	, bossTileID(0)
	, attackStatus(false)
	, attackReactionTime(0)
	, attackAnimation(false)
	, attackAnimationTimer(0)
	, stunned (false)
	, routeCounter(0)
	, routeCounter2(0)
	, isHit(false)
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

bool CEnemy::GetAnimationFlip()
{
	return enemyAnimationFlip;
}

//Enemy Update
void CEnemy::Update(CMap* map, Vector3& heroTile, vector<CGoodies*> goodyList, bool heroInvis)
{
	if(theStrategy != NULL)
	{
		theStrategy->Update(map, eneCurrTile, heroTile, direction, goodyList, routeCounter, routeCounter2, heroInvis, isHit);
		theStrategy->GetEnemyPosition((theENEMYPosition.x), (theENEMYPosition.y));
	}

	detectionGrid.erase(detectionGrid.begin(),detectionGrid.end());

	Vector3 eneDir (direction.x, -direction.y, 0);
	Vector3 enemyRight = eneDir.Cross(Vector3(0,0,1));
	Vector3 tempTile;
	bool renderUp(true), renderDown(true), renderFront(true);

	for (int i = 0; i < 8; ++i)
	{
		switch (i)
		{
		case 0:
			tempTile = eneCurrTile + eneDir;
			break;

		case 1:
			tempTile = eneCurrTile + enemyRight;
			break;

		case 2:
			tempTile = eneCurrTile - enemyRight;
			break;

		case 3:
			tempTile = eneCurrTile + eneDir + enemyRight;
			break;

		case 4:
			tempTile = eneCurrTile + eneDir - enemyRight;
			break;

		case 5:
			tempTile = eneCurrTile + eneDir * 2;
			break;

		case 6:
			tempTile = eneCurrTile + enemyRight * 2;
			break;

		case 7:
			tempTile = eneCurrTile - enemyRight * 2;
			break;
		}

		if(!(map->theScreenMap[tempTile.y][tempTile.x] == 20 || (map->theScreenMap[tempTile.y][tempTile.x] >= 28 && map->theScreenMap[tempTile.y][tempTile.x] <= 49)))
		{
			if((i == 5 && renderFront == false) || (i == 6 && renderUp == false) || (i == 7 && renderDown == false))
			{
			}

			else
			{
				detectionGrid.push_back(Vector3(tempTile.x * 32, 32 * (map->GetNumOfTiles_Height() - tempTile.y) - 32, 0));
			}
		}

		else
		{
			if (i == 0 && map->theScreenMap[tempTile.y][tempTile.x] != 43)
			{
				renderFront = false;
			}

			else if (i == 1 && map->theScreenMap[tempTile.y][tempTile.x] != 43)
			{
				renderUp = false;
			}

			else if (i == 2 && map->theScreenMap[tempTile.y][tempTile.x] != 43)
			{
				renderDown = false;
			}
		}
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
		//BRANDON'S ENEMIES
	case CMap::ENEMY_50:
		path.setWayPoints(map, 4, CMap::WAYPOINT_1, CMap::WAYPOINT_2, CMap::WAYPOINT_3, CMap::WAYPOINT_4);
		break;

	case CMap::ENEMY_51:
		path.setWayPoints(map, 4, CMap::WAYPOINT_5, CMap::WAYPOINT_6, CMap::WAYPOINT_7, CMap::WAYPOINT_8);
		break;

	case CMap::ENEMY_52:
		path.setWayPoints(map, 4, CMap::WAYPOINT_9, CMap::WAYPOINT_10, CMap::WAYPOINT_11, CMap::WAYPOINT_12);
		break;

	case CMap::ENEMY_53:
		path.setWayPoints(map, 4, CMap::WAYPOINT_13, CMap::WAYPOINT_14, CMap::WAYPOINT_15, CMap::WAYPOINT_16);
		break;

	case CMap::ENEMY_54:
		path.setWayPoints(map, 4, CMap::WAYPOINT_17, CMap::WAYPOINT_18, CMap::WAYPOINT_19, CMap::WAYPOINT_20);
		break;

	case CMap::ENEMY_55:
		path.setWayPoints(map, 4, CMap::WAYPOINT_21, CMap::WAYPOINT_22, CMap::WAYPOINT_23, CMap::WAYPOINT_24);
		break;

	case CMap::ENEMY_56:
		path.setWayPoints(map, 4, CMap::WAYPOINT_25, CMap::WAYPOINT_26, CMap::WAYPOINT_27, CMap::WAYPOINT_28);
		break;

		//SEAN's ENEMIES
	case CMap::ENEMY_60:
		path.setWayPoints(map, 4, CMap::WAYPOINT_41, CMap::WAYPOINT_42, CMap::WAYPOINT_43, CMap::WAYPOINT_44);
		break;

	case CMap::ENEMY_61:
		path.setWayPoints(map, 4, CMap::WAYPOINT_45, CMap::WAYPOINT_46, CMap::WAYPOINT_47, CMap::WAYPOINT_48);
		break;

	case CMap::ENEMYAMOURED_86:
		path.setWayPoints(map, 2, CMap::WAYPOINT_135, CMap::WAYPOINT_136);
		break;

	case CMap::ENEMYAMOURED_87:
		path.setWayPoints(map, 2, CMap::WAYPOINT_137, CMap::WAYPOINT_138);
		break;

	case CMap::ENEMYAMOURED_88:
		path.setWayPoints(map, 3, CMap::WAYPOINT_139, CMap::WAYPOINT_140, CMap::WAYPOINT_141);
		break;

	case CMap::ENEMYAMOURED_89:
		path.setWayPoints(map, 3, CMap::WAYPOINT_142, CMap::WAYPOINT_143, CMap::WAYPOINT_144);
		break;

	case CMap::ENEMYAMOURED_90:
		path.setWayPoints(map, 2, CMap::WAYPOINT_145, CMap::WAYPOINT_146);
		break;

	case CMap::ENEMYAMOURED_91:
		path.setWayPoints(map, 2, CMap::WAYPOINT_147, CMap::WAYPOINT_148);
		break;

	case CMap::ENEMYAMOURED_92:
		path.setWayPoints(map, 2, CMap::WAYPOINT_149, CMap::WAYPOINT_150);
		break;

	case CMap::ENEMYAMOURED_93:
		path.setWayPoints(map, 2, CMap::WAYPOINT_151, CMap::WAYPOINT_152);
		break;

	case CMap::ENEMYAMOURED_94:
		path.setWayPoints(map, 2, CMap::WAYPOINT_153, CMap::WAYPOINT_154);
		break;

	case CMap::ENEMYAMOURED_95:
		path.setWayPoints(map, 2, CMap::WAYPOINT_155, CMap::WAYPOINT_156);
		break;

	case CMap::BOSS_2:
		path.setWayPoints(map, 2, CMap::WAYPOINT_157, CMap::WAYPOINT_158);
		break;

		//GIGGS' ENEMIES
	case CMap::ENEMY_57:
		path.setWayPoints(map, 2, CMap::WAYPOINT_29, CMap::WAYPOINT_30);
		break;

	case CMap::ENEMY_58:
		path.setWayPoints(map, 4, CMap::WAYPOINT_31, CMap::WAYPOINT_32, CMap::WAYPOINT_33, CMap::WAYPOINT_34);
		break;

	case CMap::ENEMY_59:
		path.setWayPoints(map, 4, CMap::WAYPOINT_35, CMap::WAYPOINT_36, CMap::WAYPOINT_37, CMap::WAYPOINT_38);
		break;

	case CMap::ENEMYAMOURED_80:
		path.setWayPoints(map, 2, CMap::WAYPOINT_121, CMap::WAYPOINT_122);
		break;

		/*case CMap::ENEMYAMOURED_81:
		path.setWayPoints(map, 2, CMap::WAYPOINT_123, CMap::WAYPOINT_124);
		break;*/

	case CMap::ENEMYAMOURED_82:
		path.setWayPoints(map, 2, CMap::WAYPOINT_125, CMap::WAYPOINT_126);
		break;

	case CMap::ENEMYAMOURED_83:
		path.setWayPoints(map, 2, CMap::WAYPOINT_127, CMap::WAYPOINT_128);
		break;

	case CMap::ENEMYAMOURED_84:
		path.setWayPoints(map, 3, CMap::WAYPOINT_129, CMap::WAYPOINT_130, CMap::WAYPOINT_131);
		break;

	case CMap::ENEMYAMOURED_85:
		path.setWayPoints(map, 3, CMap::WAYPOINT_132, CMap::WAYPOINT_133, CMap::WAYPOINT_134);
		break;

		//WEI HENG'S ENEMIES
	case CMap::ENEMY_70:
		path.setWayPoints(map, 4, CMap::WAYPOINT_81, CMap::WAYPOINT_82, CMap::WAYPOINT_83, CMap::WAYPOINT_84);
		break;

	case CMap::ENEMY_71:
		path.setWayPoints(map, 4, CMap::WAYPOINT_85, CMap::WAYPOINT_86, CMap::WAYPOINT_87, CMap::WAYPOINT_88);
		break;

	case CMap::ENEMY_72:
		path.setWayPoints(map, 4, CMap::WAYPOINT_87, CMap::WAYPOINT_88, CMap::WAYPOINT_85, CMap::WAYPOINT_86);
		break;

	case CMap::ENEMY_73:
		path.setWayPoints(map, 2, CMap::WAYPOINT_89, CMap::WAYPOINT_90);
		break;

	case CMap::ENEMY_74:
		path.setWayPoints(map, 2, CMap::WAYPOINT_91, CMap::WAYPOINT_92);
		break;

	case CMap::ENEMY_75:
		path.setWayPoints(map, 2, CMap::WAYPOINT_93, CMap::WAYPOINT_94);
		break;

	case CMap::ENEMY_76:
		path.setWayPoints(map, 2, CMap::WAYPOINT_95, CMap::WAYPOINT_96);
		break;

	case CMap::ENEMY_77:
		path.setWayPoints(map, 2, CMap::WAYPOINT_97, CMap::WAYPOINT_98);
		break;
	}
}