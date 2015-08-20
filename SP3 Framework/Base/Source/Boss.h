#pragma once
#include "Map.h"

class CBoss
{
public:
	CBoss();
	~CBoss();

	enum BOSS_STATES
	{
		B_SPAWN = 0,
		B_IDLE,
		B_TOTAL
	};

	void BossInit();

	void Set_BossDestination(int boss_X, int boss_Y);																// Sets Boss Waypoint 
	void Set_SpawnGuards(bool IsTurn);																				// Sets The Spawn Point for the Guards to spawn when the boss summons it
	void Set_AnimationCounter(int moveUpStart, int moveUpEnd, int moveDownStart, int moveDownEnd, bool IsUp);		// Sets which animation to use

	int Get_BossX();
	int Get_BossY();	
	int Get_MoveUpStart();																							// Gets the First Frame of Move Up Animation
	int Get_MoveUpEnd();																							// Gets the End Frame of Move Up Animation
	int Get_MoveDownStart();																						// Gets the First frame of Move Down Animation
	int Get_MoveDownEnd();																							// Gets the End Frame of Move Down Animation
	
	bool Get_SpawnGuards();
	bool Get_IsUp();																								// Check if the animation is look up or down

	bool active;
	BOSS_STATES BossState;																							// Different Boss States

private:
	int boss_X, boss_Y;																								// Boss X and Y Position
	int moveUpStart, moveUpEnd, moveDownStart, moveDownEnd;															// Countdown to handle the up and down animation
		
	bool IsUp;																										// Check if the player is moving up or down
	bool IsTurn;
};

