#include "Minimap.h"

CMinimap::CMinimap()
	: m_cMinimap_Background(NULL)
	, m_cMinimap_Border(NULL)
	, m_cMinimap_Avatar(NULL)
	, angle(0)
	, x(0)
	, y(0)
	, size_x(0)
	, size_y(0)
	, m_cMiniMap1(NULL)
	, m_cMiniMap2(NULL)
	, m_cMiniMap3(NULL)
	, m_cMiniMap4(NULL)
	, m_cMiniMap5(NULL)
	, m_cMiniMap6(NULL)
	, m_cMiniMap7(NULL)
	, m_cMiniMap8(NULL)
	, m_cMinimap_EnemyAvatar(NULL)
	, m_cMinimap_DiamondAvatar(NULL)
	, m_cMinimap_KeyAvatar(NULL)
	, m_cMinimap_BarrelAvatar(NULL)
	, m_cMinimap_BossAvatar(NULL)
	, m_cMinimap_BossAvatar2(NULL)
{
}

CMinimap::~CMinimap(void)
{
	if (m_cMiniMap1)
	{
		delete m_cMiniMap1;
		m_cMiniMap1 = NULL;
	}

	if (m_cMiniMap2)
	{
		delete m_cMiniMap2;
		m_cMiniMap2 = NULL;
	}

	if (m_cMiniMap3)
	{
		delete m_cMiniMap3;
		m_cMiniMap3 = NULL;
	}

	if (m_cMiniMap4)
	{
		delete m_cMiniMap4;
		m_cMiniMap4 = NULL;
	}

	if (m_cMiniMap5)
	{
		delete m_cMiniMap5;
		m_cMiniMap5 = NULL;
	}

	if (m_cMiniMap6)
	{
		delete m_cMiniMap6;
		m_cMiniMap6 = NULL;
	}

	if (m_cMiniMap7)
	{
		delete m_cMiniMap7;
		m_cMiniMap7 = NULL;
	}

	if (m_cMiniMap8)
	{
		delete m_cMiniMap8;
		m_cMiniMap8 = NULL;
	}
}

// ========================== Initializing MiniMap Inits ==========================

void CMinimap::InitMiniMap_1()
{
	m_cMiniMap1 = new CMinimap();
}

void CMinimap::InitMiniMap_2()
{
	m_cMiniMap2 = new CMinimap();
}

void CMinimap::InitMiniMap_3()
{
	m_cMiniMap3 = new CMinimap();
}

void CMinimap::InitMiniMap_4()
{
	m_cMiniMap4 = new CMinimap();
}

void CMinimap::InitMiniMap_5()
{
	m_cMiniMap5 = new CMinimap();
}

void CMinimap::InitMiniMap_6()
{
	m_cMiniMap6 = new CMinimap();
}

void CMinimap::InitMiniMap_7()
{
	m_cMiniMap7 = new CMinimap();
}

void CMinimap::InitMiniMap_8()
{
	m_cMiniMap8 = new CMinimap();
}

// ========================== Set Functions ==========================

bool CMinimap::SetBackground(Mesh* aBackground)			// Set the Background Mesh to this Class
{
	if (aBackground != NULL)
	{
		m_cMinimap_Background = aBackground;
		return true;
	}
	return false;
}

bool CMinimap::SetBorder(Mesh* aBorder)
{
	if (aBorder != NULL)
	{
		m_cMinimap_Border = aBorder;
		return true;
	}
	return false;
}

bool CMinimap::SetAvatar(Mesh* anAvatar)
{
	if (anAvatar != NULL)
	{
		m_cMinimap_Avatar = anAvatar;
		return true;
	}

	return false;
}

bool CMinimap::SetEnemyAvatar(Mesh* anEnemyAvatar)
{
	if (anEnemyAvatar != NULL)
	{
		m_cMinimap_EnemyAvatar = anEnemyAvatar;
		return true;
	}

	return false;
}

bool CMinimap::SetKeyAvatar(Mesh* aKeyAvatar)
{
	if (aKeyAvatar != NULL)
	{
		m_cMinimap_KeyAvatar = aKeyAvatar;
		return true;
	}

	return false;
}

bool CMinimap::SetBarrelAvatar(Mesh* BarrelAvatar)
{
	if (BarrelAvatar != NULL)
	{
		m_cMinimap_BarrelAvatar = BarrelAvatar;
		return true;
	}

	return false;
}

bool CMinimap::SetBossAvatar(Mesh* BossAvatar)
{
	if (BossAvatar != NULL)
	{
		m_cMinimap_BossAvatar = BossAvatar;
		return true;
	}

	return false;
}

bool CMinimap::SetBossAvatar2(Mesh* BossAvatar2)
{
	if (BossAvatar2 != NULL)
	{
		m_cMinimap_BossAvatar2 = BossAvatar2;
		return true;
	}

	return false;
}

bool CMinimap::SetDiamondAvatar(Mesh* DiamondAvatar)
{
	if (DiamondAvatar != NULL)
	{
		m_cMinimap_DiamondAvatar = DiamondAvatar;
		return true;
	}

	return false;
}

bool CMinimap::SetAngle(const int angle)
{
	this->angle = angle;
	return true;
}

bool CMinimap::SetPosition(const int x, const int y)
{
	this->x = x;
	this->y = y;
	return true;
}

bool CMinimap::SetSize(const int size_x, const int size_y)
{
	this->size_x = size_x;
	this->size_y = size_y;
	return true;
}

// ========================== Get Functions ==========================

Mesh* CMinimap::GetBackground(void)
{
	return m_cMinimap_Background;
}

Mesh* CMinimap::GetBorder(void)
{
	return m_cMinimap_Border;
}

Mesh* CMinimap::GetAvatar(void)
{
	return m_cMinimap_Avatar;
}

Mesh* CMinimap::GetDiamondAvatar(void)
{
	return m_cMinimap_DiamondAvatar;
}

Mesh* CMinimap::GetEnemyAvatar(void)
{
	return m_cMinimap_EnemyAvatar;
}

Mesh* CMinimap::GetBarrelAvater(void)
{
	return m_cMinimap_BarrelAvatar;
}

Mesh* CMinimap::GetBossAvatar(void)
{
	return m_cMinimap_BossAvatar;
}

Mesh* CMinimap::GetBossAvatar2(void)
{
	return m_cMinimap_BossAvatar2;
}

Mesh* CMinimap::GetKeyAvatar(void)
{
	return m_cMinimap_KeyAvatar;
}

int CMinimap::GetAngle(void)
{
	return angle;
}

int CMinimap::GetPosition_x(void)
{
	return x;
}

int CMinimap::GetPosition_y(void)
{
	return y;
}

int CMinimap::GetSize_x(void)
{
	return size_x;
}

int CMinimap::GetSize_y(void)
{
	return size_y;
}
