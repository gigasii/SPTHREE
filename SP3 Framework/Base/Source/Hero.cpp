#include "Hero.h"
#include <math.h>

static const float Max_mapOffset_x = 1024;

Hero::Hero()
	: theHeroPositionx(0)
	, theHeroPositiony(0)
	, jumpspeed(0)
	, hero_inMidAir_Up(false)
	, hero_inMidAir_Down(false)
	, heroAnimationCounter(0)
	, heroAnimationInvert(false)
	, heroAnimationFlip(false)
	, pickUpWeapon(false)
	, attackStatus(false)
	, keyAcquired(false)
	, doorOpened(false)
	, daggerAcquired(false)
	, moveToLeft(false)
	, moveToRight(false)
	, moveToDown(false)
	, moveToUp(false)
	, heroCurrTile(0,0,0)
{
}

Hero::~Hero()
{
}

void Hero::settheHeroPositionx(int theHeroPositionx_)
{
	theHeroPositionx = theHeroPositionx_;
}

void Hero::settheHeroPositiony(int theHeroPositiony_)
{
	theHeroPositiony = theHeroPositiony_;
}

void Hero::SetAnimationInvert(bool heroAnimationInvert_)
{
	heroAnimationInvert = heroAnimationInvert_;
}

void Hero::SetAnimationCounter(int heroAnimationCounter_)
{
	heroAnimationCounter = heroAnimationCounter_;
}

void Hero::SetAttackStatus(bool attackStatus_)
{
	attackStatus = attackStatus_;
}

void Hero::SetKeyAcquired(bool keyAcquired_)
{
	keyAcquired = keyAcquired_;
}

void Hero::SetdoorOpened(bool doorOpened_)
{
	this->doorOpened = doorOpened_;
}

void Hero::SetDaggerAcquired(bool daggerAcquired_)
{
	daggerAcquired = daggerAcquired_;
}

int Hero::gettheHeroPositionx()
{
	return theHeroPositionx;
}

int Hero::gettheHeroPositiony()
{
	return theHeroPositiony;
}

bool Hero::GetAnimationInvert()
{
	return heroAnimationInvert;
}

int Hero::GetAnimationCounter()
{
	return heroAnimationCounter;
}

bool Hero::GetAnimationFlip()
{
	return heroAnimationFlip;
}

bool Hero::GetPickUpWeapon()
{
	return pickUpWeapon;
}

bool Hero::GetAttackStatus()
{
	return attackStatus;
}

bool Hero::GetKeyAcquired()
{
	return keyAcquired;
}

bool Hero::GetdoorOpened()
{
	return doorOpened;
}

bool Hero::GetDaggerAcquired()
{
	return daggerAcquired;
}

void Hero::ConstrainHero(CMap *mapType, const int leftBorder, const int rightBorder, const int topBorder, const int bottomBorder, float timeDiff)
{
	if(theHeroPositionx < leftBorder)
	{
		theHeroPositionx = leftBorder;
		if(mapType->scroll == true)
		{
			mapType->mapOffset_x = mapType->mapOffset_x - (int)(2.0f * timeDiff);
			
			if(mapType->mapOffset_x < 0)
				mapType->mapOffset_x = 0;
			
			else
				theHeroPositionx = leftBorder;
		}
	}

	else if(theHeroPositionx > rightBorder)
	{
		theHeroPositionx = rightBorder;
		if(mapType->scroll == true)
		{
			mapType->mapOffset_x = mapType->mapOffset_x + (int)(2.0f * timeDiff);
			
			if(mapType->mapOffset_x > Max_mapOffset_x)
				mapType->mapOffset_x = Max_mapOffset_x;
			
			else
				theHeroPositionx = rightBorder;
		}
	}

	if(theHeroPositiony < topBorder)
	{
		theHeroPositiony = topBorder;
	}

	else if(theHeroPositiony > bottomBorder)
	{
		theHeroPositiony = bottomBorder;
	}
}

bool Hero::CheckCollision(CMap *mapType, bool checkleft, bool checkright, bool checkdown, bool checkup)
{
	int tileTopLeft_x = (int) ((mapType->mapOffset_x + theHeroPositionx) / mapType->GetTileSize());
	int tileTopLeft_y = mapType->GetNumOfTiles_Height() - (int)ceil( (float)(theHeroPositiony + mapType->GetTileSize() + jumpspeed) / mapType->GetTileSize());

	if(checkleft)
	{
		if(mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x - 1] == CMap::WALL)
		{
			return true;
		}

		else if(mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x - 1] == CMap::DOOR)
		{
			if(this->doorOpened == false)
			{
				return true;
			}
			

		}

		if(mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x - 1] == CMap::CHEST)
		{
			pickUpWeapon = true;
			return true;
		}

		else
		{
			pickUpWeapon = false;
		}
	}

	else if(checkright)
	{
		if(mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x + 1] == CMap::WALL)
		{
			return true;
		}

		else if(mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x + 1] == CMap::DOOR)
		{
			if(this->doorOpened == false)
			{
				return true;
			}
		}

		if(mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x + 1] == CMap::CHEST)
		{
			pickUpWeapon = true;
			return true;
		}

		else
		{
			pickUpWeapon = false;
		}
	}

	else if(checkup)
	{
		if(mapType->theScreenMap[tileTopLeft_y - 1][tileTopLeft_x] == CMap::WALL)
		{
			return true;
		}

		if(mapType->theScreenMap[tileTopLeft_y - 1][tileTopLeft_x] == CMap::CHEST)
		{
			pickUpWeapon = true;
			return true;
		}

		else
		{
			pickUpWeapon = false;
		}
	}

	else if(checkdown)
	{
		if(mapType->theScreenMap[tileTopLeft_y + 1][tileTopLeft_x] == CMap::WALL)
		{
			return true;
		}

		if(mapType->theScreenMap[tileTopLeft_y + 1][tileTopLeft_x] == CMap::CHEST)
		{
			pickUpWeapon = true;
			return true;
		}

		else
		{
			pickUpWeapon = false;
		}
	}

	return false;
}

void Hero::HeroUpdate(CMap *mapType, const char key, const bool jump, int level) 
{	
	//Update walking
	if(key == 'a' && !CheckCollision(mapType, true, false, false, false) && moveToRight == false && moveToUp == false && moveToDown == false)
	{
		HeroMoveLeftRight(true, 1.0f);
		float tempCheckLeft = (float)(mapType->mapOffset_x + theHeroPositionx) / mapType->GetTileSize();

		if(moveToLeft == false)
			heroCurrTile.x -= 1;

		if(tempCheckLeft != (int)tempCheckLeft)
			moveToLeft = true;
	}

	else if(key == 'd' && !CheckCollision(mapType, false, true, false, false) && moveToLeft == false && moveToUp == false && moveToDown == false)
	{
		HeroMoveLeftRight(false, 1.0f);
		float tempCheckRight = (float)(mapType->mapOffset_x + theHeroPositionx) / mapType->GetTileSize();

		if (moveToRight == false)
			heroCurrTile.x += 1;

		if (tempCheckRight != (int)tempCheckRight)
			moveToRight = true;
	}

	if(key == 'w' && !CheckCollision(mapType, false, false, false, true) && moveToRight == false && moveToLeft == false && moveToDown == false)
	{
		HeroMoveUpDown(true, 1.0f);
		float tempCheckUp = ((float)(theHeroPositiony) / mapType->GetTileSize());

		if(moveToUp== false)
			heroCurrTile.y -= 1;

		if(tempCheckUp != (int)tempCheckUp)
			moveToUp = true;
	}

	else if(key == 's' && !CheckCollision(mapType, false, false, true, false) && moveToRight == false && moveToLeft == false && moveToUp == false)
	{
		HeroMoveUpDown(false, 1.0f);
		float tempCheckDown = ((float)(theHeroPositiony) / mapType->GetTileSize());

		if(moveToDown== false)
			heroCurrTile.y += 1;

		if(tempCheckDown != (int)tempCheckDown)
			moveToDown= true;
	}

	if(moveToLeft == true)
	{
		HeroMoveLeftRight(true, 1.0f);
		float tempCheckLeft2 = (float)(mapType->mapOffset_x + theHeroPositionx) / mapType->GetTileSize();

		if (tempCheckLeft2 == (int)tempCheckLeft2)
			moveToLeft = false;
	}

	if (moveToRight == true)
	{
		HeroMoveLeftRight(false, 1.0f);
		float tempCheckRight2 = (float)(mapType->mapOffset_x + theHeroPositionx) / mapType->GetTileSize();

		if(tempCheckRight2 == (int)tempCheckRight2)
			moveToRight = false;
	}

	if(moveToUp == true)
	{
		HeroMoveUpDown(true, 1.0f);
		float tempCheckUp2 = ((float)(theHeroPositiony) / mapType->GetTileSize());

		if(tempCheckUp2 == (int)tempCheckUp2)
			moveToUp = false;
	}

	if(moveToDown == true)
	{
		HeroMoveUpDown(false, 1.0f);
		float tempCheckDown2 = ((float)(theHeroPositiony) / mapType->GetTileSize());

		if(tempCheckDown2 == (int)tempCheckDown2)
			moveToDown = false;
	}

	/*if(level == 2)
	{
	//Update jumping
	if(jump)
	{
	HeroJump();
	}

	//Update Hero's info
	if(hero_inMidAir_Up == false && hero_inMidAir_Down == false)
	{
	//Don't jump, standing on ground - Make player fall if not standing on tile
	hero_inMidAir_Down = true;
	}

	else if(hero_inMidAir_Up == true && hero_inMidAir_Down == false)
	{
	if(CheckCollision(false, false, false, true))
	{
	//Since the new position does not allow the hero to move into, then go back to the old position
	theHeroPositiony = ((int) (theHeroPositiony / m_cMap->GetTileSize())) * m_cMap->GetTileSize();
	hero_inMidAir_Up = false;
	hero_inMidAir_Down = true;
	jumpspeed = 0;
	}

	else
	{
	UpdateJumpUP();
	}
	}

	else if(hero_inMidAir_Up == false && hero_inMidAir_Down == true)
	{
	if(CheckCollision(false, false, true, false))
	{
	//Since the new position does not allow the hero to move into, then go back to the old position
	theHeroPositiony = ((int) (theHeroPositiony / m_cMap->GetTileSize())) * m_cMap->GetTileSize();
	hero_inMidAir_Down = false;
	jumpspeed = 0;
	}

	else
	{
	UpdateFallDOWN();
	}
	}
	}*/

	if(level == 1 || level == 7)
	{
		ConstrainHero(mapType, 0, 992, 25, 740, 1.0f);
	}

	else
	{
		if(mapType->mapOffset_x > 0)
		{
			ConstrainHero(mapType, 480, 544, 25, 740, 1.0f);
		}
		
		else 
		{
			ConstrainHero(mapType, 0, 544, 25, 740, 1.0f);
		}
	}

	mapType->tileOffset_x = (int)(mapType->mapOffset_x / mapType->GetTileSize());
	if(mapType->tileOffset_x + mapType->GetNumOfTiles_Width() > mapType->getNumOfTiles_MapWidth())
	{
		mapType->tileOffset_x = mapType->getNumOfTiles_MapWidth() -mapType->GetNumOfTiles_Width();
	}
} 

void Hero::HeroJump() 
{ 
	if(hero_inMidAir_Up == false && hero_inMidAir_Down == false) 
	{ 
		hero_inMidAir_Up = true;
		jumpspeed = 15;
	} 
} 

void Hero::UpdateJumpUP()
{
	theHeroPositiony += jumpspeed;
	jumpspeed -= 1;

	if(jumpspeed == 0)
	{
		hero_inMidAir_Up = false;
		hero_inMidAir_Down = true;
	}
}

void Hero::UpdateFallDOWN()
{
	theHeroPositiony += jumpspeed;
	jumpspeed -= 1;
}

void Hero::HeroMoveUpDown(const bool mode, const float timeDiff) 
{ 
	if(mode) 
	{ 
		theHeroPositiony = theHeroPositiony + (int) (2.0f * timeDiff);
		heroAnimationFlip = true;
	}  

	else 
	{ 
		theHeroPositiony = theHeroPositiony - (int) (2.0f * timeDiff);
		heroAnimationFlip = false;
	}
} 

void Hero::HeroMoveLeftRight(const bool mode, const float timeDiff) 
{ 
	if(mode) 
	{ 
		theHeroPositionx = theHeroPositionx - (int) (2.0f * timeDiff);
		heroAnimationInvert = true;
	}

	else 
	{ 
		theHeroPositionx = theHeroPositionx + (int) (2.0f * timeDiff);
		heroAnimationInvert = false;
	}
}