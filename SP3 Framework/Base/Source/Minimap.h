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
	Mesh* m_cMinimap_EnemyAvatar;
	Mesh* m_cMinimap_DiamondAvatar;
	Mesh* m_cMinimap_KeyAvatar;
	Mesh* m_cMinimap_BarrelAvatar;
	Mesh* m_cMinimap_BossAvatar;
	Mesh* m_cMinimap_BossAvatar2;

	bool SetBackground(Mesh* aBackground);				// Set the background mesh to this class
	bool SetBorder(Mesh* aBorder);						// Set the Border mesh to this class
	bool SetAvatar(Mesh* anAvatar);						// Set the Avatar mesh to this class
	bool SetAngle(const int angle);						// Set Angle
	bool SetPosition(const int x, const int y);			// Set Position
	bool SetSize(const int size_x, const int size_y);	// Set Size
	bool SetEnemyAvatar(Mesh* anEnemyAvatar);			// Set Enemy Avatar on Minimap
	bool SetDiamondAvatar(Mesh* DiamondAvatar);			// Set The Avatar for Diamond on the Minimap
	bool SetKeyAvatar(Mesh* KeyAvatar);					// Set The Avatar for Key on the Minimap
	bool SetBarrelAvatar(Mesh* BarrelAvatar);			// Set The Avatar for Barrel on the Minimap
	bool SetBossAvatar(Mesh* BossAvatar);				// Set The Avatar for Boss on the Minimap
	bool SetBossAvatar2(Mesh* BossAvatar2);

	void InitMiniMap_1();
	void InitMiniMap_2();
	void InitMiniMap_3();
	void InitMiniMap_4();
	void InitMiniMap_5();
	void InitMiniMap_6();
	void InitMiniMap_7();
	void InitMiniMap_8();

	Mesh* GetBackground(void);							// Get the background mesh to this class
	Mesh* GetBorder(void);								// Get the Border mesh to this class
	Mesh* GetAvatar(void);								// Get the Avatar mesh to this class
	Mesh* GetEnemyAvatar(void);							// Get the Enemy Avatar Mesh to this Class
	Mesh* GetDiamondAvatar(void);						// Get the Diamond Avatar Mesh to this Class
	Mesh* GetKeyAvatar(void);							// Get the Key Avatar Mesh to this class
	Mesh* GetBarrelAvater(void);						// Get the Barrel Avater Mesh to this class
	Mesh* GetBossAvatar(void);
	Mesh* GetBossAvatar2(void);

	// Minimaps Init
	CMinimap* m_cMiniMap1;
	CMinimap* m_cMiniMap2;
	CMinimap* m_cMiniMap3;
	CMinimap* m_cMiniMap4;
	CMinimap* m_cMiniMap5;
	CMinimap* m_cMiniMap6;
	CMinimap* m_cMiniMap7;
	CMinimap* m_cMiniMap8;

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

