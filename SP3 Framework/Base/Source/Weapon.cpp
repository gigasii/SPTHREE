#include "Weapon.h"

CWeapon::CWeapon()
	: daggerAcquired(false)
	, shurikensAcquired(false)
	, attackSpeed(0)
	, ammo(2)
	, rotation(0, -1, 0)
	, shurikenTileID(0)
{
}

CWeapon::~CWeapon()
{
}

void CWeapon::SetDaggerAcquired(bool daggerAcquired_)
{
	daggerAcquired = daggerAcquired_;
}

bool CWeapon::GetDaggerAcquired()
{
	return daggerAcquired;
}

void CWeapon::SetShurikensAcquired(bool shurikensAcquired_)
{
	shurikensAcquired = shurikensAcquired_;
}

bool CWeapon::GetShurikensAcquired()
{
	return shurikensAcquired;
}