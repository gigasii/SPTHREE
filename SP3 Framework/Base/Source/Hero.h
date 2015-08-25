#ifndef HERO_H
#define HERO_H

#include <iostream>
#include <Vector2.h>
#include "Map.h"
#include "Enemy.h"
#include "Goodies.h"

class Hero
{

private:
	//Hero's information
	int  theHeroPositionx;
	int  theHeroPositiony;
	bool heroAnimationInvert;
	bool heroAnimationFlip;
	bool moveToLeft, moveToRight, moveToUp, moveToDown;

	//Hero map interaction
	bool keyAcquired;
	bool doorOpened;
	
	//Weapon stuff
	bool pickUpWeapon;
	bool daggerAcquired;
	bool attackStatus;

public:
	Hero();
	~Hero();

	Vector3 heroCurrTile;
	Vector3 direction;

	float heroTileID;
	int health;
	bool hiding;

	//Set the position x of the player
	void settheHeroPositionx(int theHeroPositionx_);
	//Get the position x of the player
	int gettheHeroPositionx();

	//Set the position of y of the player
	void settheHeroPositiony(int settheHeroPositiony_);
	//Get position y of the player
	int gettheHeroPositiony();

	//Get Animation Invert status of the palyer
	bool GetAnimationInvert();

	//Get Animation flip of the player
	bool GetAnimationFlip();

	//Check whether a weapon has been picked up
	bool GetPickUpWeapon();

	//Set attack status of player
	void SetAttackStatus(bool attackStatus_);
	//Get attack status of player
	bool GetAttackStatus();

	//Set whether key has been taken
	void SetKeyAcquired(bool keyAcquired_);
	//Check whether key has been taken
	bool GetKeyAcquired();

	//Set whether key has been taken
	void SetdoorOpened(bool doorOpened_);
	//Check whether key has been taken
	bool GetdoorOpened();

	//Set whether dagger has been taken
	void SetDaggerAcquired(bool daggerAcquired_);
	//Check whether dagger has been taken
	bool GetDaggerAcquired();

	//Constrain the position of the Hero to within the border
	void ConstrainHero(CMap *mapType, const int leftBorder, const int rightBorder, const int topBorder, const int bottomBorder, float timeDiff);

	//Check for collision of hero with obstacles in a certain direction
	bool CheckCollision(CMap *mapType, vector<CGoodies*>, vector<CEnemy*>, bool checkleft, bool checkright, bool checkdown, bool checkup);

	//Methods for Hero
	void HeroUpdate(CMap *mapType, vector<CGoodies*>, vector<CEnemy*>, const char key, int level);
	void HeroMoveUpDown(const bool mode, const float timeDiff);
	void HeroMoveLeftRight(const bool mode, const float timeDiff);
};

#endif