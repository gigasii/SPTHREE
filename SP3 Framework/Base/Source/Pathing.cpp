#include "Pathing.h"


CPathing::CPathing(void) : location(0), dir(DOWN), xFix(false), yFix(xFix)
{
	for(std::vector<Vector3>::iterator it = WayPointList.begin(); it!= WayPointList.end(); ++it)
	{
		Vector3 go = (Vector3)*it;
		go.SetZero();
	}

	for(std::vector<Vector3>::iterator it = WayPointTileList.begin(); it!= WayPointTileList.end(); ++it)
	{
		Vector3 go = (Vector3)*it;
		go.SetZero();
	}
}


CPathing::~CPathing(void)
{
}

void CPathing::setWayPoints(CMap* map, int amount, ...)
{
	Vector3 temp;

	va_list wayPoints;
	va_start(wayPoints, amount);

	for (short itr = 0; itr < amount; ++itr)
	{
		//Set Point to current Parameter;
		int Point = va_arg(wayPoints, int);

		//Loop through column
		for (short i = 0; i < map->getNumOfTiles_MapHeight(); ++i)
		{
			//Loop through row
			for (short j = 0; j < map->getNumOfTiles_MapWidth()+1; ++j)
			{
				//Process Tiles
				if (map->theScreenMap[i][j] == Point)
				{
					//Set Way Points

					temp.Set((j * map->GetTileSize() - map->mapOffset_x), 
						             map->GetTileSize() * (map->GetNumOfTiles_Height() - i) - map->GetTileSize(),0);

					WayPointList.push_back(temp);
					WayPointTileList.push_back(Vector3(j,i,0));
				}
			}
		} //End of for loop
	}

	va_end(wayPoints);
}

CPathing::Direction CPathing::getDir()
{
	return this->dir;
}

void CPathing::executePath(float &x, float &y, float speed)
{
	//Caters for Buffer Zone
	short Buffer = 5;

	//Fixes X Movement Shaking Problem
	if (!xFix)
	{
		//Move to Way Point's X Pos
		if ((int)x > WayPointList[location].x)
		{
			x -= speed;
			dir = LEFT;
		}
		else if ((int)x < WayPointList[location].x)
		{
			x += speed;
			dir = RIGHT;
		}

		//Disallow X Movement when target is reached
		if ((int)x < WayPointList[location].x+Buffer && (int)x > WayPointList[location].x-Buffer)
			xFix = true;
	}

	//Fixes Y Movement Shaking Problem
	if (!yFix)
	{
		//Move to Way Point's Y Pos
		if ((int)y > WayPointList[location].y)
		{
			y -= speed;
			dir = UP;
		}
		else if ((int)y < WayPointList[location].y)
		{
			y += speed;
			dir = DOWN;
		}

		//Disallow Y Movement when target is reached
		if ((int)y < WayPointList[location].y+Buffer && (int)y > WayPointList[location].y-Buffer)
			yFix = true;
	}

	//Reset xFix flag to reallow X Movement
	if ((int)y < WayPointList[location].y+Buffer && (int)y > WayPointList[location].y-Buffer)
		xFix = false;

	//Reset yFix flag to reallow Y Movement
	if ((int)x < WayPointList[location].x+Buffer && (int)x > WayPointList[location].x-Buffer)
		yFix = false;

	//Increment Location (Buffer of 5)
	if (((int)x < WayPointList[location].x+Buffer && (int)x > WayPointList[location].x-Buffer) && 
		((int)y < WayPointList[location].y+Buffer && (int)y > WayPointList[location].y-Buffer))
		++location;

	//Reset Location
	if (location > (short)(WayPointList.size()-1))
		location = 0;
}
