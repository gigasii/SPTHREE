#include "Hero.h"
#include <math.h>

static const float Max_mapOffset_x = 1024;

Hero::Hero()
	: theHeroPositionx(0)
	, theHeroPositiony(0)
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
	, heroTileID(0)
	, health(3)
	, hiding(false)
	, direction(0,-1,0)
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
			mapType->mapOffset_x = mapType->mapOffset_x - (int)(4.0f * timeDiff);
			
			if(mapType->mapOffset_x < 0)
			{
				mapType->mapOffset_x = 0;
			}
			
			else
			{
				theHeroPositionx = leftBorder;
			}
		}
	}

	else if(theHeroPositionx > rightBorder)
	{
		theHeroPositionx = rightBorder;	
		if(mapType->scroll == true)
		{
			mapType->mapOffset_x = mapType->mapOffset_x + (int)(4.0f * timeDiff);
			
			if(mapType->mapOffset_x > Max_mapOffset_x)
			{
				mapType->mapOffset_x = Max_mapOffset_x;
			}
			
			else
			{
				theHeroPositionx = rightBorder;
			}
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

bool Hero::CheckCollision(CMap *mapType, vector<CGoodies*> obstacles, vector<CEnemy*> enemyList, bool checkleft, bool checkright, bool checkdown, bool checkup)
{
	int tileTopLeft_x = (int)((mapType->mapOffset_x + theHeroPositionx) / mapType->GetTileSize());
	int tileTopLeft_y = mapType->GetNumOfTiles_Height() - (int)ceil((float)(theHeroPositiony + mapType->GetTileSize()) / mapType->GetTileSize());

	if(checkleft)
	{
		for(std::vector<CEnemy *>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
		{
			CEnemy *go = (CEnemy *)*it;
			if(go->active)	
			{
				if((theHeroPositionx - go->GetPos_x() + mapType->mapOffset_x <= 32 && theHeroPositionx - go->GetPos_x() + mapType->mapOffset_x >= 0) && (theHeroPositiony - go->GetPos_y() >= 0 && theHeroPositiony - go->GetPos_y() <= 1)
					|| Vector3(heroCurrTile.y, heroCurrTile.x - 1) == Vector3(go->eneCurrTile.y, go->eneCurrTile.x))
				{
					return true;
				}
			}
		}

	   if(mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x - 1] == CMap::DOOR)
		{
			if(this->doorOpened == false)
			{
				return true;
			}

			else
			{
				return false;
			}
	   }

	   if(mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x - 1] == CMap::BOSS)
	   {
		   return true;
	   }

	   if(mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x - 1] == CMap::HAY)
	   {
		   return false;
	   }

	   if(mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x - 1] == CMap::BARREL)
	   {
		   for(std::vector<CGoodies *>::iterator it = obstacles.begin(); it != obstacles.end(); ++it)
		   {
			   CGoodies *go = (CGoodies *)*it;
			   int tile_x = go->GetPos_x() / mapType->GetTileSize();
			   int tile_y = mapType->GetNumOfTiles_Height() - (go->GetPos_y() + mapType->GetTileSize()) / mapType->GetTileSize();

			   if(tile_y == tileTopLeft_y)
			   {
				   if(tile_x == tileTopLeft_x - 1)
				   {
					   if(go->active)
					   {
						   return true;
					   }
					   
					   else
					   {
						   return false;
					   }
				   }
			   }
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

		if(mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x - 1] > 3)
		{
			if((mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x - 1] >= 21 && mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x - 1] <= 27) || mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x - 1]  >= 50)
			{
			}
			
			else
			{
				return true;
			}
		}
	}

	else if(checkright)
	{
		for(std::vector<CEnemy *>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
		{
			CEnemy *go = (CEnemy *)*it;
			if(go->active)	
			{
				if((theHeroPositionx - go->GetPos_x() + mapType->mapOffset_x <= 0 && theHeroPositionx - go->GetPos_x() + mapType->mapOffset_x >= -32) && (theHeroPositiony - go->GetPos_y() >= 0 && theHeroPositiony - go->GetPos_y() <= 1)
					|| Vector3(heroCurrTile.y, heroCurrTile.x + 1) == Vector3(go->eneCurrTile.y, go->eneCurrTile.x))
				{
					return true;
				}
			}
		}

		if(mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x + 1] == CMap::DOOR)
		{
			if(this->doorOpened == false)
			{
				return true;
			}

			else
			{
				return false;
			}
		}

		if(mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x + 1] == CMap::HAY)
	    {
			return false;
	    }

		if(mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x + 1] == CMap::BARREL)
		{
			for(std::vector<CGoodies *>::iterator it = obstacles.begin(); it != obstacles.end(); ++it)
			{
				CGoodies *go = (CGoodies *)*it;
				int tile_x = go->GetPos_x() / mapType->GetTileSize();
				int tile_y = mapType->GetNumOfTiles_Height() - (go->GetPos_y() + mapType->GetTileSize()) / mapType->GetTileSize();

				if(tile_y == tileTopLeft_y)
				{
					if(tile_x == tileTopLeft_x + 1)
					{
						if(go->active)
						{
							return true;
						}
						
						else
						{
							return false;
						}
					}
				}
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

		if(mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x + 1] > 3)
		{
			if((mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x + 1] >= 21 && mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x + 1] <= 27) || mapType->theScreenMap[tileTopLeft_y][tileTopLeft_x + 1]  >= 50)
			{
			}
			
			else
			{
				return true;
			}
		}
	}

	else if(checkup)
	{
		for(std::vector<CEnemy *>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
		{
			CEnemy *go = (CEnemy *)*it;
			if(go->active)	
			{
				if((theHeroPositionx - go->GetPos_x() + mapType->mapOffset_x >= 0 && theHeroPositionx - go->GetPos_x() + mapType->mapOffset_x <= 1) && (theHeroPositiony - go->GetPos_y() >= -32 && theHeroPositiony - go->GetPos_y() <= 0)
					|| Vector3(heroCurrTile.y - 1, heroCurrTile.x) == Vector3(go->eneCurrTile.y, go->eneCurrTile.x))
				{
					return true;
				}
			}
		}

		if(mapType->theScreenMap[tileTopLeft_y - 1][tileTopLeft_x] == CMap::BOSS)
		{
			return true;
		}

		if(mapType->theScreenMap[tileTopLeft_y - 1][tileTopLeft_x] == CMap::HAY)
	    {
			return false;
	    }

		if(mapType->theScreenMap[tileTopLeft_y - 1][tileTopLeft_x] == CMap::BARREL)
		{
			for(std::vector<CGoodies *>::iterator it = obstacles.begin(); it != obstacles.end(); ++it)
			{
				CGoodies *go = (CGoodies *)*it;
				int tile_x = go->GetPos_x() / mapType->GetTileSize();
				int tile_y = mapType->GetNumOfTiles_Height() - (go->GetPos_y() + mapType->GetTileSize()) / mapType->GetTileSize();

				if(tile_y == tileTopLeft_y - 1)
				{
					if(tile_x == tileTopLeft_x)
					{
						if(go->active)
						{
							return true;
						}

						else
						{
							return false;
						}
					}
				}
			}
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

		if(mapType->theScreenMap[tileTopLeft_y - 1][tileTopLeft_x] > 3)
		{
			if((mapType->theScreenMap[tileTopLeft_y - 1][tileTopLeft_x] >= 21 && mapType->theScreenMap[tileTopLeft_y - 1][tileTopLeft_x] <= 27) || mapType->theScreenMap[tileTopLeft_y - 1][tileTopLeft_x]  >= 50)
			{
			}

			else
			{
				return true;
			}
		}
	}

	else if(checkdown)
	{
		for(std::vector<CEnemy *>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
		{
			CEnemy *go = (CEnemy *)*it;
			if(go->active)	
			{
				if((theHeroPositionx - go->GetPos_x() + mapType->mapOffset_x >= 0 && theHeroPositionx - go->GetPos_x() + mapType->mapOffset_x <= 1) && (theHeroPositiony - go->GetPos_y() >= 0 && theHeroPositiony - go->GetPos_y() <= 32)
					|| Vector3(heroCurrTile.y + 1, heroCurrTile.x) == Vector3(go->eneCurrTile.y, go->eneCurrTile.x))
				{
					return true;
				}
			}
		}

		if(mapType->theScreenMap[tileTopLeft_y + 1][tileTopLeft_x] == CMap::BOSS)
		{
			return true;
		}

		if(mapType->theScreenMap[tileTopLeft_y + 1][tileTopLeft_x] == CMap::HAY)
	    {
			return false;
	    }

		if(mapType->theScreenMap[tileTopLeft_y + 1][tileTopLeft_x] == CMap::BARREL)
		{
			for(std::vector<CGoodies *>::iterator it = obstacles.begin(); it != obstacles.end(); ++it)
			{
				CGoodies *go = (CGoodies *)*it;
				int tile_x = go->GetPos_x() / mapType->GetTileSize();
				int tile_y = mapType->GetNumOfTiles_Height() - (go->GetPos_y() + mapType->GetTileSize()) / mapType->GetTileSize();

				if(tile_y == tileTopLeft_y + 1)
				{
					if(tile_x == tileTopLeft_x)
					{
						if(go->active)
						{
							return true;
						}
						
						else
						{
							return false;
						}
					}
				}
			}
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

		if(mapType->theScreenMap[tileTopLeft_y + 1][tileTopLeft_x] > 3)
		{
			if((mapType->theScreenMap[tileTopLeft_y + 1][tileTopLeft_x] >= 21 && mapType->theScreenMap[tileTopLeft_y + 1][tileTopLeft_x] <= 27) || mapType->theScreenMap[tileTopLeft_y + 1][tileTopLeft_x] >= 50)
			{
			}

			else
			{
				return true;
			}
		}	
	}

	return false;
}

void Hero::HeroUpdate(CMap *mapType, vector<CGoodies*> obtacles, vector<CEnemy*> enemyList, const char key, int level) 
{	
	if(moveToRight == false && moveToLeft == false && moveToDown == false && moveToUp == false)
	{
		if(key == 'a' && !CheckCollision(mapType, obtacles, enemyList, true, false, false, false))
		{
			HeroMoveLeftRight(true, 1.0f);
			float tempCheckLeft = (float)(mapType->mapOffset_x + theHeroPositionx) / mapType->GetTileSize();

			if(moveToLeft == false)
			{
				heroCurrTile.x -= 1;
			}

			if(tempCheckLeft != (int)tempCheckLeft)
			{
				moveToLeft = true;
			}
		}

		else if(key == 'd' && !CheckCollision(mapType, obtacles, enemyList, false, true, false, false))
		{
			HeroMoveLeftRight(false, 1.0f);
			float tempCheckRight = (float)(mapType->mapOffset_x + theHeroPositionx) / mapType->GetTileSize();

			if(moveToRight == false)
			{
				heroCurrTile.x += 1;
			}

			if(tempCheckRight != (int)tempCheckRight)
			{
				moveToRight = true;
			}
		}

		if(key == 'w' && !CheckCollision(mapType, obtacles, enemyList, false, false, false, true))
		{
			HeroMoveUpDown(true, 1.0f);
			float tempCheckUp = ((float)(theHeroPositiony) / mapType->GetTileSize());

			if(moveToUp == false)
			{
				heroCurrTile.y -= 1;
			}

			if(tempCheckUp != (int)tempCheckUp)
			{
				moveToUp = true;
			}
		}

		else if(key == 's' && !CheckCollision(mapType, obtacles, enemyList, false, false, true, false))
		{
			HeroMoveUpDown(false, 1.0f);
			float tempCheckDown = ((float)(theHeroPositiony) / mapType->GetTileSize());

			if(moveToDown== false)
			{
				heroCurrTile.y += 1;
			}

			if(tempCheckDown != (int)tempCheckDown)
			{
				moveToDown= true;
			}
		}
	}

	if(moveToLeft == true)
	{
		HeroMoveLeftRight(true, 1.0f);
		float tempCheckLeft2 = (float)(mapType->mapOffset_x + theHeroPositionx) / mapType->GetTileSize();

		if(tempCheckLeft2 == (int)tempCheckLeft2)
		{
			moveToLeft = false;
		}
	}

	if(moveToRight == true)
	{
		HeroMoveLeftRight(false, 1.0f);
		float tempCheckRight2 = (float)(mapType->mapOffset_x + theHeroPositionx) / mapType->GetTileSize();

		if(tempCheckRight2 == (int)tempCheckRight2)
		{
			moveToRight = false;
		}
	}

	if(moveToUp == true)
	{
		HeroMoveUpDown(true, 1.0f);
		float tempCheckUp2 = ((float)(theHeroPositiony) / mapType->GetTileSize());

		if(tempCheckUp2 == (int)tempCheckUp2)
		{
			moveToUp = false;
		}
	}

	if(moveToDown == true)
	{
		HeroMoveUpDown(false, 1.0f);
		float tempCheckDown2 = ((float)(theHeroPositiony) / mapType->GetTileSize());

		if(tempCheckDown2 == (int)tempCheckDown2)
		{
			moveToDown = false;
		}
	}

	//Screen maps
	if(level == 1 || level == 3 || level == 5 || level == 7)
	{
		ConstrainHero(mapType, 0, 992, 25, 740, 1.0f);
	}

	//Scrolling maps
	else
	{
		if(mapType->mapOffset_x > 0)
		{
			if(mapType->mapOffset_x >= 1024)
			{
				ConstrainHero(mapType, 480, 992, 25, 740, 1.0f);
			}
			
			else
			{
				ConstrainHero(mapType, 480, 544, 25, 740, 1.0f);
			}
		}
		
		else 
		{
			ConstrainHero(mapType, 0, 544, 25, 740, 1.0f);
		}
	}

	mapType->tileOffset_x = (int)(mapType->mapOffset_x / mapType->GetTileSize());
	if(mapType->tileOffset_x + mapType->GetNumOfTiles_Width() > mapType->getNumOfTiles_MapWidth())
	{
		mapType->tileOffset_x = mapType->getNumOfTiles_MapWidth() - mapType->GetNumOfTiles_Width();
	}
}

void Hero::HeroMoveUpDown(const bool mode, const float timeDiff) 
{ 
	if(mode) 
	{ 
		theHeroPositiony = theHeroPositiony + (int) (4.0f * timeDiff);
		heroAnimationFlip = true;
	}  

	else 
	{ 
		theHeroPositiony = theHeroPositiony - (int) (4.0f * timeDiff);
		heroAnimationFlip = false;
	}
} 

void Hero::HeroMoveLeftRight(const bool mode, const float timeDiff) 
{ 
	if(mode) 
	{ 
		theHeroPositionx = theHeroPositionx - (int) (4.0f * timeDiff);
		heroAnimationInvert = true;
	}

	else 
	{ 
		theHeroPositionx = theHeroPositionx + (int) (4.0f * timeDiff);
		heroAnimationInvert = false;
	}
}