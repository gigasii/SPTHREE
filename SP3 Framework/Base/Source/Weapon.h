#pragma once
#include <iostream>
#include <Vector3.h>

class CWeapon
{

private:
	bool daggerAcquired;
	bool shurikensAcquired;

public:
	CWeapon();
	~CWeapon();

	//Dagger
	float attackSpeed;

	//Shuriken
	int ammo;
	Vector3 rotation;
	float shurikenTileID;

	//Set whether dagger has been taken
	void SetDaggerAcquired(bool daggerAcquired_);
	//Check whether dagger has been taken
	bool GetDaggerAcquired();

	//Set whether Shurikens have been taken
	void SetShurikensAcquired(bool shurikensAcquired_);
	//Check whether Shurikens have been taken
	bool GetShurikensAcquired();
};

