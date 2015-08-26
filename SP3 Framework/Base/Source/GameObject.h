#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_WALL,
		GO_PILLAR,
		GO_AIM,
		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 normal;
	bool active;
	float mass;
	bool isSet;
	bool render;
	float rotate;
	int ID;
	float timer;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
	void reset(void);
};

#endif