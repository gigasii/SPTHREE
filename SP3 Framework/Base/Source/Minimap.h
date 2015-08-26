#pragma once
#include "Mesh.h"

class CMinimap
{
public:
	CMinimap();
	virtual ~CMinimap(void);

	Mesh* m_cMinimap_Background;
	Mesh* m_cMinimap_Border;
	Mesh* m_cMinimap_Avatar;

	bool SetBackground(Mesh* aBackground);				// Set the background mesh to this class
	bool SetBorder(Mesh* aBorder);						// Set the Border mesh to this class
	bool SetAvatar(Mesh* anAvatar);						// Set the Avatar mesh to this class
	bool SetAngle(const int angle);
	bool SetPosition(const int x, const int y);
	bool SetSize(const int size_x, const int size_y);

	Mesh* GetBackground(void);							// Get the background mesh to this class
	Mesh* GetBorder(void);								// Get the Border mesh to this class
	Mesh* GetAvatar(void);								// Get the Avatar mesh to this class


	int GetAngle(void);									// Get Angle
	int GetPosition_x(void);							// Get Position X Of Avatar In Minimap
	int GetPosition_y(void);							// Get Position Y Of Avatar In Minimap
	int GetSize_x(void);								// Get Size Of Minimap (For Calculation of Avatar in Minimap);
	int GetSize_y(void);								// Get Size of Minimap (For Calculation of Avatar in Minimap);

private:
	int angle;											// Rotation from First Angle
	int x, y;											// Offset in the Minimap
	int size_x, size_y;									// Minimap Size
};

