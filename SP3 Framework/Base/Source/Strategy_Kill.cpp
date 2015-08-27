#include "Strategy_Kill.h"

CStrategy_Kill::CStrategy_Kill() : oldTile(0,0,0), currTile(0,0,0), checkMoved(true)
{
}

CStrategy_Kill::~CStrategy_Kill()
{
}

void CStrategy_Kill::Update(CMap* map, Vector3& enemyTile, Vector3& heroTile, Vector3& enemyDir, vector<CGoodies*> goodyList, int &routeCounter, int &routeCounter2, bool heroInvis)
{
	//Decide which state to change to
	int distanceHeroToEnemy = CalculateDistance();
	
	for (int i = 0; i < map->getNumOfTiles_MapHeight(); ++i)
	{
		for (int j = 0; j < map->getNumOfTiles_MapWidth(); ++j)
		{
			Map[j][i] = map->theScreenMap[i][j];
		}
	}
	
	if (detectionCheck(map, enemyTile, heroTile, enemyDir, heroInvis))
	{
		isAttacking = true;
	}

	else if (routeCounter == 0 && isAttacking == false)
	{
		if (CurrentState != PATROL)
		{
			theEnemyPath.location = 0;

			route2 = pathFind(enemyTile.x, enemyTile.y, 
					theEnemyPath.WayPointTileList[theEnemyPath.location].x, 
					theEnemyPath.WayPointTileList[theEnemyPath.location].y,
					goodyList);

		}

		CurrentState = PATROL;
	}

	if (isAttacking == true)
	{
		if(distanceHeroToEnemy < 1024.f)
			CurrentState = REPEL;

		else if (routeCounter2 == 0)
			CurrentState = ATTACK;

		if(distanceHeroToEnemy > 29729.0f && routeCounter2 == 0)
			isAttacking = false;
	}

	//Based on the current state, move the enemy
	switch(CurrentState)
	{
	case ATTACK:
		{
			oldTile = currTile;
			currTile = heroTile;

			if (currTile != oldTile)
			{
				checkMoved = true;
			}

			if (checkMoved == true)
			{
				if (routeCounter == 0)
				{	
					route = pathFind(enemyTile.x, enemyTile.y, heroTile.x, heroTile.y,goodyList);
					checkMoved = false;
				}
			}

			for (int i = 0; i < route.length(); ++i)
			{
				char temp = route[i];

				if (temp != '9')
				{
					switch (temp)
					{
					case '4':
						theEnemyPosition.x += 1;
						theEnemyPosition.y -= 1;
						break;
					case '1':
						theEnemyPosition.y -= 4;

						enemyDir = Vector3(0,-1,0);

						if (routeCounter == 0)
							enemyTile.y += 1;

						break;
					case '6':
						theEnemyPosition.x -= 1;
						theEnemyPosition.y -= 1;
						break;
					case '2':
						theEnemyPosition.x -= 4;

						enemyDir = Vector3(-1,0,0);

						if (routeCounter == 0)
							enemyTile.x -= 1; 

						break;
					case '5':
						theEnemyPosition.x -= 1;
						theEnemyPosition.y += 1;
						break;
					case '3':
						theEnemyPosition.y += 4;

						enemyDir = Vector3(0,1,0);

						if (routeCounter == 0)
							enemyTile.y -= 1;

						break;
					case '7':
						theEnemyPosition.x += 1;
						theEnemyPosition.y += 1;
						break;
					case '0':
						theEnemyPosition.x += 4;

						enemyDir = Vector3(1,0,0);

						if (routeCounter == 0)
							enemyTile.x += 1;  

						break;
					}

					routeCounter++;

					if (routeCounter >= 8)
					{
						route[i] = '9';
						routeCounter = 0;
					}

					break;
				}

			}
		}
		break;

	case REPEL:
		/*theEnemyPosition.x = theEnemyPosition.x + (theDestination.x - theEnemyPosition.x <= 0 ? 1 : - 1);
		theEnemyPosition.y = theEnemyPosition.y + (theDestination.y - theEnemyPosition.y <= 0 ? 1 : - 1);*/
		break;

	case PATROL:
		/*theEnemyPath.executePath(theEnemyPosition.x,theEnemyPosition.y,2);*/
		{
			if (enemyTile.x == theEnemyPath.WayPointTileList[theEnemyPath.location].x &&
				enemyTile.y == theEnemyPath.WayPointTileList[theEnemyPath.location].y && 
				routeCounter2 == 0)
			{
				theEnemyPath.location++;

				if (theEnemyPath.location > (short)(theEnemyPath.WayPointTileList.size()-1))
						theEnemyPath.location = 0;

				route2 = pathFind(enemyTile.x, enemyTile.y, 
					theEnemyPath.WayPointTileList[theEnemyPath.location].x, 
					theEnemyPath.WayPointTileList[theEnemyPath.location].y,
					goodyList);
			}

			for (int i = 0; i < route2.length(); ++i)
			{
				char temp2 = route2[i];

				if (temp2 != '9')
				{
					switch (temp2)
					{
					case '1':
						theEnemyPosition.y -= 2;

						enemyDir = Vector3(0,-1,0);

						if (routeCounter2 == 0)
							enemyTile.y += 1;

						break;
					
					case '2':
						theEnemyPosition.x -= 2;

						enemyDir = Vector3(-1,0,0);

						if (routeCounter2 == 0)
							enemyTile.x -= 1; 

						break;
					case '3':
						theEnemyPosition.y += 2;

						enemyDir = Vector3(0,1,0);

						if (routeCounter2 == 0)
							enemyTile.y -= 1;

						break;
				
					case '0':
						theEnemyPosition.x += 2;

						enemyDir = Vector3(1,0,0);

						if (routeCounter2 == 0)
							enemyTile.x += 1;  

						break;
					}

					routeCounter2++;

					if (routeCounter2 >= 16)
					{
						route2[i] = '9';
						routeCounter2 = 0;
					}

					break;
				}

			}
		}

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

string CStrategy_Kill::pathFind(const int& xStart, const int& yStart, const int& xFinish, const int& yFinish,vector<CGoodies*> goodyList)
{
	static priority_queue<CNode> myQueue[2];
	static int queueIndex;
	static CNode* tempNode1;
	static CNode* tempNode2;
    static int i, j, x, y, xdx, ydy;
    static char c;

	queueIndex = 0;

	//reset node maps
	for(y = 0; y < m; y++)
	{
		for (x = 0; x < n; x++)
		{
			closed_nodes_map[x][y]=0;
            open_nodes_map[x][y]=0;
		}
	}

	//create start node and put into open list
	tempNode1 = new CNode (xStart, yStart, 0, 0);
	tempNode1->updatePriority(xFinish, yFinish);

	myQueue[queueIndex].push(*tempNode1);
	open_nodes_map[x][y] = tempNode1->getPriority();

	//A* search
	while(!myQueue[queueIndex].empty())
	{
		//get current node w/ highest priority
		tempNode1 = new CNode (myQueue[queueIndex].top().getPosX(), myQueue[queueIndex].top().getPosY(),
							   myQueue[queueIndex].top().getLevel(), myQueue[queueIndex].top().getPriority());

		x = tempNode1->getPosX();
		y = tempNode1->getPosY();

		myQueue[queueIndex].pop(); //remove node from open list
		open_nodes_map[x][y] = 0;
		closed_nodes_map[x][y] = 1;

		//stop searching when goal is reached
		if (x == xFinish && y == yFinish)
		{
			//generate the path
			string path = "";
			
			while (!(x == xStart && y == yStart))
			{
				j = dir_map[x][y];
				c = '0' + (j + dir/2) % dir;
				path = c + path;
				x += dx[j];
				y += dy[j];
			}

			delete tempNode1;

			while (!myQueue[queueIndex].empty())
				myQueue[queueIndex].pop();

			return path;
		}

		//generate moves in all directions
		for (i = 0; i < dir; i++)
		{
			xdx = x + dx[i];
			ydy = y + dy[i];
		

			if (!(xdx < 0 || xdx > n - 1 || ydy < 0 || ydy > m - 1 ||  Map[xdx][ydy] == CMap::CHEST || closed_nodes_map[xdx][ydy] == 1 ||
				!((Map[xdx][ydy] >= 21 && Map[xdx][ydy] <= 27) || Map[xdx][ydy] <= 0 || Map[xdx][ydy] == CMap::BARREL || Map[xdx][ydy] >= CMap::ENEMY_50 ||
				   Map[xdx][ydy] == CMap::KEY ||  Map[xdx][ydy] == CMap::JEWEL || Map[xdx][ydy] == CMap::HAY )))
			{
				//if (Map[xdx][ydy] == CMap::BARREL)
				//{
				//	for(std::vector<CGoodies *>::iterator it = goodyList.begin(); it != goodyList.end(); ++it)
				//	{
				//		CGoodies *go = (CGoodies *)*it;

				//		if (go->tilePos == Vector3(xdx,ydy,0) && go->active == false)
				//		{
				//			goto pathfind;
				//		}
				//	}
				//}

				//else
				//{
					//pathfind:
					//generate child node
					tempNode2 = new CNode (xdx, ydy, tempNode1->getLevel(), tempNode1->getPriority());
					tempNode2->nextLevel(i);
					tempNode2->updatePriority(xFinish, yFinish);

					//add if not already in open list
					if (open_nodes_map[xdx][ydy] == 0)
					{
						//update priority
						open_nodes_map[xdx][ydy] = tempNode2->getPriority();
						myQueue[queueIndex].push(*tempNode2);
						//update direction
						dir_map[xdx][ydy] = (i + dir/2) % dir;
					}

					else if(open_nodes_map[xdx][ydy] > tempNode2->getPriority())
					{
						// update the priority info
						open_nodes_map[xdx][ydy] = tempNode2->getPriority();
						// update the parent direction info
						dir_map[xdx][ydy]=(i+dir/2)%dir;
						//replace the node
						while (!(myQueue[queueIndex].top().getPosX() == xdx &&
							myQueue[queueIndex].top().getPosY() == ydy))
						{
							myQueue[1 - queueIndex].push(myQueue[queueIndex].top());
							myQueue[queueIndex].pop();
						}

						myQueue[queueIndex].pop();

						if (myQueue[queueIndex].size() > myQueue[1 - queueIndex].size())
							queueIndex = 1 - queueIndex;

						while (!myQueue[queueIndex].empty())
						{
							myQueue[1 - queueIndex].push(myQueue[queueIndex].top());
							myQueue[queueIndex].pop();
						}

						queueIndex = 1 - queueIndex;
						myQueue[queueIndex].push(*tempNode2);
					}

					else
						delete tempNode2;
				//}
			}
		}

		delete tempNode1;
	}

	return ""; // no path found
}

bool CStrategy_Kill::detectionCheck(CMap* map, Vector3& enemyTile, Vector3& heroTile, Vector3& enemyDir, bool heroInvis)
{
	if (map->theScreenMap[heroTile.y][heroTile.x] == CMap::HAY || heroInvis == true)
		return false;

	Vector3 tempTile;
	Vector3 enemyDir2 (enemyDir.x, -enemyDir.y, 0);
	Vector3 enemyRight = enemyDir2.Cross(Vector3(0,0,1));

	if (enemyTile + enemyDir2 == heroTile || enemyTile + enemyDir2 + enemyRight == heroTile || 
		enemyTile + enemyDir2 - enemyRight == heroTile || enemyTile + enemyRight == heroTile || 
		enemyTile - enemyRight == heroTile)
	{
		return true;
	}

	tempTile = enemyTile + enemyDir2;

	if (!(map->theScreenMap[tempTile.y][tempTile.x] >= 44 && map->theScreenMap[tempTile.y][tempTile.x] <= 49) &&
		enemyTile + (enemyDir2 * 2) == heroTile)
	{
		return true;
	}

	tempTile = enemyTile + enemyRight;

	if (!(map->theScreenMap[tempTile.y][tempTile.x] >= 44 && map->theScreenMap[tempTile.y][tempTile.x] <= 49) &&
		enemyTile + (enemyRight * 2) == heroTile)
	{
		return true;
	}

	tempTile = enemyTile - enemyRight;

	if (!(map->theScreenMap[tempTile.y][tempTile.x] >= 44 && map->theScreenMap[tempTile.y][tempTile.x] <= 49) &&
		enemyTile - (enemyRight * 2) == heroTile)
	{
		return true;
	}

	return false;
}
