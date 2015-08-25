#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "Goodies.h"
#include "GameObject.h"

class CEnemy;
using namespace std;

class CMap
{

public:
	CMap(void);
	~CMap(void);

	enum tileList
	{
		// Enemy List: 50 - 59	   Waypoint: 001 - 40		Brandon 
		// Enemy List: 60 - 69	   Waypoint: 041 - 80		Sean
		// Enemy List: 70 - 79	   Waypoint: 081 - 120		Wei Heng
		// Enemy List: 80 - ...	   Waypoint: 121 - 160		Giggs

		GROUND	= 0,
		WALL	= 1,
		JEWEL	= 2,
		KEY		= 3,
		CHEST	= 4,
		BARREL	= 5,
		DOOR	= 6,
		HAY		= 7,
		HOLE	= 8,
		BOSS	= 17,

		//BRANDON's ENEMIES
		ENEMY_50 = 50, WAYPOINT_1 = -1, WAYPOINT_2 = -2, WAYPOINT_3 = -3, WAYPOINT_4 = -4,
		ENEMY_51 = 51, WAYPOINT_5 = -5, WAYPOINT_6 = -6, WAYPOINT_7 = -7, WAYPOINT_8 = -8,
		ENEMY_52 = 52, WAYPOINT_9 = -9, WAYPOINT_10 = -10, WAYPOINT_11 = -11, WAYPOINT_12 = -12,
		ENEMY_53 = 53, WAYPOINT_13 = -13, WAYPOINT_14 = -14, WAYPOINT_15 = -15, WAYPOINT_16 = -16,
		ENEMY_54 = 54, WAYPOINT_17 = -17, WAYPOINT_18 = -18, WAYPOINT_19 = -19, WAYPOINT_20 = -20,
		ENEMY_55 = 55, WAYPOINT_21 = -21, WAYPOINT_22 = -22, WAYPOINT_23 = -23, WAYPOINT_24 = -24,
		ENEMY_56 = 56, WAYPOINT_25 = -25, WAYPOINT_26 = -26, WAYPOINT_27 = -27, WAYPOINT_28 = -28,

		//SEAN's ENEMIES
		ENEMY_60 = 60, WAYPOINT_41 = -41, WAYPOINT_42 = -42, WAYPOINT_43 = -43, WAYPOINT_44 = -44,
		ENEMY_61 = 61, WAYPOINT_45 = -45, WAYPOINT_46 = -46, WAYPOINT_47 = -47, WAYPOINT_48 = -48,
		
		//GIGGS' ENEMIES
		ENEMYAMOURED_80 = 80, WAYPOINT_121 = -121, WAYPOINT_122 = -122,
		ENEMYAMOURED_81 = 81, WAYPOINT_123 = -123, WAYPOINT_124 = -124,
		ENEMYAMOURED_82 = 82, WAYPOINT_125 = -125, WAYPOINT_126 = -126,

		TILE_TOTAL
	};

	void Init(const int theScreen_Height, const int theScreen_Width, const int theNumOfTiles_Height, const int theNumOfTiles_Width, const int theMap_Height, const int theMap_Width, const int theTileSize = 32);
	bool LoadMap(const string mapName);
	int GetNumOfTiles_Height(void);			//Get the number of tiles for the height of the screen
	int GetNumOfTiles_Width(void);			//Get the number of tiles for width of the screen
	int GetTileSize(void);

	int getNumOfTiles_MapHeight(void);		//Get the number of tiles for the height of the map
	int getNumOfTiles_MapWidth(void);		//Get the number of tiles for width of the map

	void InitScreenMap(vector<CEnemy*> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList, std::vector<GameObject *> &goList);
	void InitScrollingMap(std::vector<CEnemy *> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList);
	void InitBossMap(vector<CEnemy*> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList, std::vector<CGoodies *> &HoleList);
	void InitCustomMap();
	void InitScreenMap3(vector<CEnemy*> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList);

	//Codes for scrolling
	int mapOffset_x, mapOffset_y;
	int tileOffset_x, tileOffset_y;
	int mapFineOffset_x, mapFineOffset_y;

	//Number of ties in the map's width
	int theNumOfTiles_MapWidth;

	//Screen height
	int theScreen_Height;

	//Map Inits
	CMap *m_cScreenMap;
	CMap *m_cScreenMap3;
	CMap *m_cScrollingMap;
	CMap *m_cBossMap;
	CMap *m_cCustomMap;

	vector<vector<int> > theScreenMap;
	bool scroll;

private:
	int theScreen_Width;				//Width of the screen
	int theNumOfTiles_Height;			//Number of tiles in the screen's height
	int theNumOfTiles_Width;			//Number of tiles in the screen's width
	int theTileSize;

	int theMap_Height;					//The map's height
	int theMap_Width;					//The map's width
	int theNumOfTiles_MapHeight;		//Number of tiles in the map's height

	bool LoadFile(const string mapName);
};
