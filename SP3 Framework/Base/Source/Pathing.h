#pragma once

#include "Vector3.h"
#include "Map.h"
#include <cstdarg>

using namespace::std;

class CPathing
{
public:
	CPathing(void);
	~CPathing(void);

	enum Direction
	{
		UP, RIGHT, LEFT, DOWN
	};

	//Set Points
	void setWayPoints(CMap* map, int amount, ...);

	//Follow along waypoints
	void executePath(float &x, float &y, float speed);

	//Vector List to store Way Points
	std::vector<Vector3> WayPointList;
	short location;

	//Get Direction
	Direction getDir();

private:
	bool xFix, yFix;
	Direction dir;

};

