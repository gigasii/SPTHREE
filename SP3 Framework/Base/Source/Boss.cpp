#include "Boss.h"


CBoss::CBoss() 
	: moveUpStart(0)
	, moveUpEnd(0)
	, moveDownStart(0)
	, moveDownEnd(0)
	, IsUp(false)
	, active(true)
	, BossState(BOSS_STATES::B_SPAWN)
{
}

CBoss::~CBoss()
{
}

void CBoss::BossInit()
{
	boss_X = 0;
	boss_Y = 0;
}

void CBoss::Set_BossDestination(int boss_X, int boss_Y)
{
	this->boss_X = boss_X;
	this->boss_Y = boss_Y;
}

void CBoss::Set_SpawnGuards(bool IsTurn)
{
	this->IsTurn = IsTurn;

	if (IsTurn == true)
		BossState = B_SPAWN;
	else
		BossState = B_IDLE;
}

bool CBoss::Get_SpawnGuards()
{
	return IsTurn;
}

int CBoss::Get_BossX()
{
	return boss_X;
}

int CBoss::Get_BossY()
{
	return boss_Y;
}

// ================================= Boss Animation Function =================================

void CBoss::Set_AnimationCounter(int moveUpStart, int moveUpEnd, int moveDownStart, int moveDownEnd, bool IsUp)
{
	this->IsUp = IsUp;

	if (IsUp == true)
	{
		this->moveUpStart = moveUpStart;
		this->moveUpEnd = moveUpEnd;
	}
	else
	{
		this->moveDownStart = moveDownStart;
		this->moveDownEnd = moveDownEnd;
	}
}

int CBoss::Get_MoveUpStart()
{
	return moveUpStart;
}

int CBoss::Get_MoveUpEnd()
{
	return moveUpEnd;
}

int CBoss::Get_MoveDownStart()
{
	return moveDownStart;
}

int CBoss::Get_MoveDownEnd()
{
	return moveDownEnd;
}

bool CBoss::Get_IsUp()
{
	return IsUp;
}