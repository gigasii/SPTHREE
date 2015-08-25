#include "Map.h"
#include "Enemy.h"
#include "GameObject.h"

CMap::CMap(void)
: theScreen_Height(0)
, theScreen_Width(0)
, theNumOfTiles_Height(0)
, theNumOfTiles_Width(0)
, theMap_Height(0)
, theMap_Width(0)
, theNumOfTiles_MapHeight(0)
, theNumOfTiles_MapWidth(0)
, theTileSize(0)
, mapOffset_x(0)
, mapOffset_y(0)
, tileOffset_x(0)
, tileOffset_y(0)
, mapFineOffset_x(0)
, mapFineOffset_y(0)
, m_cScreenMap(NULL)
, m_cScreenMap3(NULL)
, m_cScrollingMap(NULL)
, m_cScrollingMap3(NULL)
, m_cBossMap(NULL)
, m_cCustomMap(NULL)
, scroll(false)
{
	theScreenMap.clear();
}

CMap::~CMap(void)
{
	if(m_cScreenMap)
	{
		delete m_cScreenMap;
		m_cScreenMap = NULL;
	}

	if(m_cScreenMap3)
	{
		delete m_cScreenMap3;
		m_cScreenMap3 = NULL;
	}

	if(m_cScrollingMap)
	{
		delete m_cScrollingMap;
		m_cScrollingMap = NULL;
	}

	if(m_cScrollingMap3)
	{
		delete m_cScrollingMap3;
		m_cScrollingMap3 = NULL;
	}

	if(m_cBossMap)
	{
		delete m_cBossMap;
		m_cBossMap = NULL;
	}

	if(m_cCustomMap)
	{
		delete m_cCustomMap;
		m_cCustomMap = NULL;
	}

	theScreenMap.clear();
}

void CMap::Init(const int theScreen_Height, const int theScreen_Width, const int theNumOfTiles_Height, const int theNumOfTiles_Width, const int theMap_Height, const int theMap_Width, int theTileSize)
{
	this->theScreen_Height		= theScreen_Height;
	this->theScreen_Width		= theScreen_Width;
	this->theNumOfTiles_Height	= theNumOfTiles_Height;
	this->theNumOfTiles_Width	= theNumOfTiles_Width;
	this->theMap_Height			= theMap_Height;
	this->theMap_Width			= theMap_Width;
	this->theTileSize			= theTileSize;

	theNumOfTiles_MapHeight = (int)(theMap_Height / theTileSize);
	theNumOfTiles_MapWidth = (int)(theMap_Width / theTileSize);

	theScreenMap.resize(theNumOfTiles_MapHeight);
	for(int i = 0; i < theNumOfTiles_MapHeight; ++i)
		theScreenMap[i].resize(theNumOfTiles_MapWidth);
}

void CMap::InitScreenMap(std::vector<CEnemy *> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList, std::vector<GameObject *> &goList)
{
	m_cScreenMap = new CMap();
	m_cScreenMap->Init(800, 1024, 25, 32, 800, 1024, 32);
	m_cScreenMap->LoadMap("Image//Level_1-1.csv");
	m_cScreenMap->scroll = false;
	int tempType;
	CEnemy* tempEnemy;
	CGoodies* tempGoodies;

	for(int i = 0; i < m_cScreenMap->getNumOfTiles_MapHeight(); ++i)
	{
		for(int j = 0; j < m_cScreenMap->getNumOfTiles_MapWidth(); ++j)
		{
			tempType = m_cScreenMap->theScreenMap[i][j];

			if(tempType == CMap::JEWEL)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScreenMap->GetTileSize(), m_cScreenMap->GetTileSize() * (m_cScreenMap->GetNumOfTiles_Height() - i) -  m_cScreenMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}
			
			else if(tempType == CMap::KEY)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScreenMap->GetTileSize(), m_cScreenMap->GetTileSize() * (m_cScreenMap->GetNumOfTiles_Height() - i) -  m_cScreenMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::KEY;	
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::CHEST)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScreenMap->GetTileSize(), m_cScreenMap->GetTileSize() * (m_cScreenMap->GetNumOfTiles_Height() - i) -  m_cScreenMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::CHEST;	
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::HAY)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScreenMap->GetTileSize(), m_cScreenMap->GetTileSize() * (m_cScreenMap->GetNumOfTiles_Height() - i) -  m_cScreenMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::HAY;	
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::BARREL)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScreenMap->GetTileSize(), m_cScreenMap->GetTileSize() * (m_cScreenMap->GetNumOfTiles_Height() - i) -  m_cScreenMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::BARREL;	
				tempGoodies->tilePos.Set(j,i,0);
				BarrelsList.push_back(tempGoodies);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType >= CMap::ENEMY_50)
			{
				tempEnemy = new CEnemy();
				tempEnemy->ChangeStrategy(NULL,false);
				tempEnemy->SetPos_x(j * m_cScreenMap->GetTileSize());
				tempEnemy->SetPos_y(m_cScreenMap->GetTileSize() * (m_cScreenMap->GetNumOfTiles_Height() - i) -  m_cScreenMap->GetTileSize());
				tempEnemy->active = true;
				tempEnemy->ID = tempType;
				tempEnemy->health = 2;
				tempEnemy->setWayPoints(m_cScreenMap);
				tempEnemy->eneCurrTile = Vector3(j,i,0);
				enemyList.push_back(tempEnemy);
				
				GameObject* go = new GameObject(GameObject::GO_PILLAR);
				goList.push_back(go);
				go->active = true;
				go->pos.Set(tempEnemy->GetPos_x(),tempEnemy->GetPos_y(),0);
				go->scale.Set(16,16,16);
				go->ID = tempType;
			}

			else if (tempType >= 29 && tempType <= 49)
			{
			}
		}
	}
}

void CMap::InitScrollingMap(std::vector<CEnemy *> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList)
{
	m_cScrollingMap = new CMap();
	m_cScrollingMap->Init(800, 1024, 25, 32, 800, 2048, 32);
	m_cScrollingMap->LoadMap("Image//Level_1-2.csv");
	m_cScrollingMap->scroll = true;
	int tempType;
	CEnemy* tempEnemy;
	CGoodies* tempGoodies;

	for(int i = 0; i < m_cScrollingMap->getNumOfTiles_MapHeight(); ++i)
	{
		for(int j = 0; j < m_cScrollingMap->getNumOfTiles_MapWidth(); ++j)
		{
			tempType = m_cScrollingMap->theScreenMap[i][j];

			if(tempType == CMap::JEWEL)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScrollingMap->GetTileSize(), m_cScreenMap->GetTileSize() * (m_cScrollingMap->GetNumOfTiles_Height() - i) - m_cScrollingMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}
			
			else if(tempType == CMap::KEY)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScrollingMap->GetTileSize(), m_cScrollingMap->GetTileSize() * (m_cScrollingMap->GetNumOfTiles_Height() - i) - m_cScrollingMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::KEY;	
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::CHEST)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScrollingMap->GetTileSize(), m_cScrollingMap->GetTileSize() * (m_cScrollingMap->GetNumOfTiles_Height() - i) - m_cScrollingMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::CHEST;	
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::BARREL)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScrollingMap->GetTileSize(), m_cScrollingMap->GetTileSize() * (m_cScrollingMap->GetNumOfTiles_Height() - i) - m_cScrollingMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::BARREL;
				tempGoodies->tilePos.Set(j,i,0);
				BarrelsList.push_back(tempGoodies);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::HAY)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScrollingMap->GetTileSize(), m_cScrollingMap->GetTileSize() * (m_cScrollingMap->GetNumOfTiles_Height() - i) - m_cScrollingMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::HAY;
				tempGoodies->tilePos.Set(j,i,0);
				//BarrelsList.push_back(tempGoodies);
				GoodiesList.push_back(tempGoodies);
			}

			else if (tempType >= CMap::ENEMY_50)
			{
				tempEnemy = new CEnemy();
				tempEnemy->ChangeStrategy(NULL,false);
				tempEnemy->SetPos_x(j * m_cScrollingMap->GetTileSize());
				tempEnemy->SetPos_y(m_cScrollingMap->GetTileSize() * (m_cScrollingMap->GetNumOfTiles_Height() - i) - m_cScrollingMap->GetTileSize());
				tempEnemy->active = true;
				tempEnemy->ID = tempType;
				tempEnemy->health = 2;
				tempEnemy->setWayPoints(m_cScrollingMap);
				tempEnemy->eneCurrTile = Vector3(j,i,0);
				enemyList.push_back(tempEnemy);
			}
		}
	}
}

void CMap::InitScreenMap3(std::vector<CEnemy *> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList)
{
	m_cScreenMap3 = new CMap();
	m_cScreenMap3->Init(800, 1024, 25, 32, 800, 1024, 32);
	m_cScreenMap3->LoadMap("Image//Level_3-1.csv");
	m_cScreenMap3->scroll = false;
	int tempType;
	CEnemy* tempEnemy;
	CGoodies* tempGoodies;

	for(int i = 0; i < m_cScreenMap3->getNumOfTiles_MapHeight(); ++i)
	{
		for(int j = 0; j < m_cScreenMap3->getNumOfTiles_MapWidth(); ++j)
		{
			tempType = m_cScreenMap3->theScreenMap[i][j];

			if(tempType == CMap::JEWEL)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScreenMap3->GetTileSize(), m_cScreenMap3->GetTileSize() * (m_cScreenMap3->GetNumOfTiles_Height() - i) - m_cScreenMap3->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::KEY)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScreenMap3->GetTileSize(), m_cScreenMap3->GetTileSize() * (m_cScreenMap3->GetNumOfTiles_Height() - i) - m_cScreenMap3->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::KEY;	
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::CHEST)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScreenMap3->GetTileSize(), m_cScreenMap3->GetTileSize() * (m_cScreenMap3->GetNumOfTiles_Height() - i) - m_cScreenMap3->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::CHEST;	
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::BARREL)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScreenMap3->GetTileSize(), m_cScreenMap3->GetTileSize() * (m_cScreenMap3->GetNumOfTiles_Height() - i) - m_cScreenMap3->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::BARREL;	
				tempGoodies->tilePos.Set(j,i,0);
				BarrelsList.push_back(tempGoodies);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::HAY)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScreenMap3->GetTileSize(), m_cScreenMap3->GetTileSize() * (m_cScreenMap3->GetNumOfTiles_Height() - i) - m_cScreenMap3->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::HAY;	
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			if(tempType >= CMap::ENEMY_50 && tempType < CMap::ENEMYAMOURED_80)
			{
				tempEnemy = new CEnemy();
				tempEnemy->ChangeStrategy(NULL,false);
				tempEnemy->SetPos_x(j * m_cScreenMap3->GetTileSize());
				tempEnemy->SetPos_y(m_cScreenMap3->GetTileSize() * (m_cScreenMap3->GetNumOfTiles_Height() - i) - m_cScreenMap3->GetTileSize());
				tempEnemy->active = true;
				tempEnemy->ID = tempType;
				tempEnemy->health = 2;
				tempEnemy->setWayPoints(m_cScreenMap3);
				tempEnemy->eneCurrTile = Vector3(j,i,0);	
				enemyList.push_back(tempEnemy);
			}

			else if(tempType >= CMap::ENEMYAMOURED_80)
			{
				tempEnemy = new CEnemy();
				tempEnemy->ChangeStrategy(NULL,false);
				tempEnemy->SetPos_x(j * m_cScreenMap3->GetTileSize());
				tempEnemy->SetPos_y(m_cScreenMap3->GetTileSize() * (m_cScreenMap3->GetNumOfTiles_Height() - i) - m_cScreenMap3->GetTileSize());
				tempEnemy->active = true;
				tempEnemy->ID = tempType;
				tempEnemy->health = 3;
				tempEnemy->setWayPoints(m_cScreenMap3);
				tempEnemy->eneCurrTile = Vector3(j,i,0);	
				enemyList.push_back(tempEnemy);
			}
		}
	}
}

void CMap::InitScrollingMap3(std::vector<CEnemy *> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList)
{
	m_cScrollingMap3 = new CMap();
	m_cScrollingMap3->Init(800, 1024, 25, 32, 800, 2048, 32);
	m_cScrollingMap3->LoadMap("Image//Level_3-2.csv");
	m_cScrollingMap3->scroll = true;
	int tempType;
	CEnemy* tempEnemy;
	CGoodies* tempGoodies;

	for(int i = 0; i < m_cScrollingMap3->getNumOfTiles_MapHeight(); ++i)
	{
		for(int j = 0; j < m_cScrollingMap3->getNumOfTiles_MapWidth(); ++j)
		{
			tempType = m_cScrollingMap3->theScreenMap[i][j];

			if(tempType == CMap::JEWEL)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScrollingMap3->GetTileSize(), m_cScrollingMap3->GetTileSize() * (m_cScrollingMap3->GetNumOfTiles_Height() - i) - m_cScrollingMap3->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}
			
			else if(tempType == CMap::KEY)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScrollingMap3->GetTileSize(), m_cScrollingMap3->GetTileSize() * (m_cScrollingMap3->GetNumOfTiles_Height() - i) - m_cScrollingMap3->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::KEY;	
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::CHEST)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScrollingMap3->GetTileSize(), m_cScrollingMap3->GetTileSize() * (m_cScrollingMap3->GetNumOfTiles_Height() - i) - m_cScrollingMap3->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::CHEST;	
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::BARREL)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScrollingMap3->GetTileSize(), m_cScrollingMap3->GetTileSize() * (m_cScrollingMap3->GetNumOfTiles_Height() - i) - m_cScrollingMap3->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::BARREL;
				tempGoodies->tilePos.Set(j,i,0);
				BarrelsList.push_back(tempGoodies);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::HAY)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cScrollingMap3->GetTileSize(), m_cScrollingMap3->GetTileSize() * (m_cScrollingMap3->GetNumOfTiles_Height() - i) - m_cScrollingMap3->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::HAY;
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			if(tempType >= CMap::ENEMY_50 && tempType < CMap::ENEMYAMOURED_80)
			{
				tempEnemy = new CEnemy();
				tempEnemy->ChangeStrategy(NULL,false);
				tempEnemy->SetPos_x(j * m_cScrollingMap3->GetTileSize());
				tempEnemy->SetPos_y(m_cScrollingMap3->GetTileSize() * (m_cScrollingMap3->GetNumOfTiles_Height() - i) - m_cScrollingMap3->GetTileSize());
				tempEnemy->active = true;
				tempEnemy->ID = tempType;
				tempEnemy->health = 2;
				tempEnemy->setWayPoints(m_cScrollingMap3);
				tempEnemy->eneCurrTile = Vector3(j,i,0);	
				enemyList.push_back(tempEnemy);
			}

			else if(tempType >= CMap::ENEMYAMOURED_80)
			{
				tempEnemy = new CEnemy();
				tempEnemy->ChangeStrategy(NULL,false);
				tempEnemy->SetPos_x(j * m_cScrollingMap3->GetTileSize());
				tempEnemy->SetPos_y(m_cScrollingMap3->GetTileSize() * (m_cScrollingMap3->GetNumOfTiles_Height() - i) - m_cScrollingMap3->GetTileSize());
				tempEnemy->active = true;
				tempEnemy->ID = tempType;
				tempEnemy->health = 3;
				tempEnemy->setWayPoints(m_cScrollingMap3);
				tempEnemy->eneCurrTile = Vector3(j,i,0);	
				enemyList.push_back(tempEnemy);
			}
		}
	}
}

void CMap::InitBossMap(vector<CEnemy*> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList, std::vector<CGoodies *> &HoleList)
{
	m_cBossMap = new CMap();
	m_cBossMap->Init(800, 1024, 25, 32, 800, 1024, 32);
	m_cBossMap->LoadMap("Image//Level_Boss.csv");
	m_cBossMap->scroll = false;
	int tempType;
	CEnemy* tempEnemy;
	CGoodies* tempGoodies;

	for(int i = 0; i < m_cBossMap->getNumOfTiles_MapHeight(); ++i)
	{
		for(int j = 0; j < m_cBossMap->getNumOfTiles_MapWidth(); ++j)
		{
			tempType = m_cBossMap->theScreenMap[i][j];

			if(tempType == CMap::JEWEL)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cBossMap->GetTileSize(), m_cBossMap->GetTileSize() * (m_cBossMap->GetNumOfTiles_Height() - i) - m_cBossMap->GetTileSize());
				tempGoodies->active = true;
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::KEY)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cBossMap->GetTileSize(), m_cBossMap->GetTileSize() * (m_cBossMap->GetNumOfTiles_Height() - i) - m_cBossMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::KEY;
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::CHEST)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cBossMap->GetTileSize(), m_cBossMap->GetTileSize() * (m_cBossMap->GetNumOfTiles_Height() - i) - m_cBossMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::CHEST;
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::HAY)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cBossMap->GetTileSize(), m_cBossMap->GetTileSize() * (m_cBossMap->GetNumOfTiles_Height() - i) - m_cBossMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::HAY;
				tempGoodies->tilePos.Set(j, i, 0);
				GoodiesList.push_back(tempGoodies);
			}

			else if (tempType == CMap::BARREL)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cBossMap->GetTileSize(), m_cBossMap->GetTileSize() * (m_cBossMap->GetNumOfTiles_Height() - i) - m_cBossMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::BARREL;
				tempGoodies->tilePos.Set(j, i, 0);
				BarrelsList.push_back(tempGoodies);
				GoodiesList.push_back(tempGoodies);
			}

			else if (tempType == CMap::HOLE)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * m_cBossMap->GetTileSize(), m_cBossMap->GetTileSize() * (m_cBossMap->GetNumOfTiles_Height() - i) - m_cBossMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::HOLE;
				tempGoodies->tilePos.Set(j, i, 0);
				BarrelsList.push_back(tempGoodies);
				GoodiesList.push_back(tempGoodies);
			}

			if (tempType >= CMap::ENEMY_50)
			{
				tempEnemy = new CEnemy();
				tempEnemy->ChangeStrategy(NULL, false);
				tempEnemy->SetPos_x(j * m_cBossMap->GetTileSize());
				tempEnemy->SetPos_y(m_cBossMap->GetTileSize() * (m_cBossMap->GetNumOfTiles_Height() - i) - m_cBossMap->GetTileSize());
				tempEnemy->active = true;
				tempEnemy->ID = tempType;
				tempEnemy->health = 2;
				tempEnemy->setWayPoints(m_cBossMap);
				tempEnemy->eneCurrTile = Vector3(j, i, 0);
				enemyList.push_back(tempEnemy);
			}
		}
	}
}

void CMap::InitCustomMap()
{
	m_cCustomMap = new CMap();
	m_cCustomMap->Init(800, 1024, 25, 32, 800, 1024, 32);
	m_cCustomMap->LoadMap("Image//MapDesign_CustomMap.csv");
	m_cCustomMap->scroll = false;
}

bool CMap::LoadMap(const string mapName)
{
	if(LoadFile(mapName) == true)
	{
		printf("Map (%s) has been successfully loaded!\n", mapName.c_str());
		return true;
	}

	return false;
}

bool CMap::LoadFile(const string mapName)
{
	int theLineCounter = 0;
	int theMaxNumOfColumns = 0;

	ifstream file(mapName.c_str());
	if(file.is_open())
	{
		int i = 0, k = 0;
		while(file.good())
		{
			string aLineOfText = "";
			getline(file, aLineOfText);

			if (theLineCounter>theNumOfTiles_MapHeight)
				break;

			// If this line is not a comment line, then process it
			if(!(aLineOfText.find("//*") == NULL) && aLineOfText != "")
			{
				if (theLineCounter == 0)
				{
					// This is the first line of the map data file
					string token;
					istringstream iss(aLineOfText);
					while(getline(iss, token, ','))
					{
						// Count the number of columns
						theMaxNumOfColumns = atoi(token.c_str());
					}
					if( theMaxNumOfColumns != theNumOfTiles_MapWidth)
						return false;
				}
				else
				{
					int theColumnCounter = 0;

					string token;
					istringstream iss(aLineOfText);
					while(getline(iss, token, ',') && (theColumnCounter<theNumOfTiles_MapWidth))
					{
						theScreenMap[theLineCounter - 1][theColumnCounter++] = atoi(token.c_str());
					}
				}
			}

			theLineCounter++;
		}
	}
	
	return true;
}

int CMap::GetNumOfTiles_Height(void)
{
	return theNumOfTiles_Height;
}

int CMap::GetNumOfTiles_Width(void)
{
	return theNumOfTiles_Width;
}

int CMap::GetTileSize(void)
{
	return theTileSize;
}

int CMap::getNumOfTiles_MapHeight(void)
{
	return theNumOfTiles_MapHeight;
}

int CMap::getNumOfTiles_MapWidth(void)
{
	return theNumOfTiles_MapWidth;
}