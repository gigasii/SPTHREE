#include "Strategy_Kill.h"

CStrategy_Kill::CStrategy_Kill() : routeCounter(0)
{
}

CStrategy_Kill::~CStrategy_Kill()
{
}

void CStrategy_Kill::Update(CMap* map)
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

	if(distanceHeroToEnemy < 100000.0f)
	{
		if(distanceHeroToEnemy < 625.0f)
		{
			//CurrentState = REPEL;
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
		{
			int EnePosX = (int) ceil((float)(map->mapOffset_x + theEnemyPosition.x) / map->GetTileSize());
			int EnePosY = map->GetNumOfTiles_Height() - (int)ceil((float)(theEnemyPosition.y + map->GetTileSize()) / map->GetTileSize());

			int oldDesX = DesPosX;
			int oldDesY = DesPosY;

			DesPosX = (int) ceil ((float)(map->mapOffset_x + theDestination.x) / map->GetTileSize());
			DesPosY = map->GetNumOfTiles_Height() - (int) ceil ((float)(theDestination.y + map->GetTileSize()) / map->GetTileSize());


			if (DesPosX == oldDesX && DesPosY == oldDesY)
			{
			}
			else
			{	
				route = pathFind(EnePosX, EnePosY, DesPosX, DesPosY);
				routeCounter = 0;
				cout << route;
				cout << " Enemy posY " << EnePosY;
				cout << " Hero posY " << DesPosY;
				cout << endl;
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
						theEnemyPosition.y -= 1;
						break;
					case '6':
						theEnemyPosition.x -= 1;
						theEnemyPosition.y -= 1;
						break;
					case '2':
						theEnemyPosition.x -= 1;
						break;
					case '5':
						theEnemyPosition.x -= 1;
						theEnemyPosition.y += 1;
						break;
					case '3':
						theEnemyPosition.y += 1;
						break;
					case '7':
						theEnemyPosition.x += 1;
						theEnemyPosition.y += 1;
						break;
					case '0':
						theEnemyPosition.x += 1;
						break;
					}

					routeCounter++;

					if (routeCounter >= 32)
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
			theEnemyPosition.x = theEnemyPosition.x + (theDestination.x - theEnemyPosition.x <= 0 ? 1 : - 1);
			theEnemyPosition.y = theEnemyPosition.y + (theDestination.y - theEnemyPosition.y <= 0 ? 1 : - 1);
			break;
		
		case PATROL:
			theEnemyPath.executePath(theEnemyPosition.x,theEnemyPosition.y,1);
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

string CStrategy_Kill::pathFind(const int& xStart, const int& yStart, const int& xFinish, const int& yFinish)
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
		

			if (!(xdx < 0 || xdx > n - 1 || ydy < 0 || ydy > m - 1 || Map[xdx][ydy] == 1 || Map[xdx][ydy] == 4 || closed_nodes_map[xdx][ydy] == 1))
			{
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
			}
		}

		delete tempNode1;
	}

	return ""; // no path found
}

