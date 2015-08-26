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
{
}


CMinimap::~CMinimap(void)
{
}

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
