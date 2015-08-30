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
, m_cBossScrollingMap(NULL)
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

	if (m_cBossScrollingMap)
	{
		delete m_cBossScrollingMap;
		m_cBossScrollingMap = NULL;
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
	//m_cScreenMap->LoadMap("Image//Level_Shop.csv");
	m_cScreenMap->scroll = false;
	setMap(m_cScreenMap, enemyList, GoodiesList, BarrelsList, goList);
}

void CMap::InitScrollingMap(std::vector<CEnemy *> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList, std::vector<GameObject *> &goList)
{
	m_cScrollingMap = new CMap();
	m_cScrollingMap->Init(800, 1024, 25, 32, 800, 2048, 32);
	m_cScrollingMap->LoadMap("Image//Level_1-2.csv");
	m_cScrollingMap->scroll = true;
	setMap(m_cScrollingMap, enemyList, GoodiesList, BarrelsList, goList);
}

void CMap::InitScreenMap2(std::vector<CEnemy *> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList, std::vector<GameObject *> &goList)
{
	m_cScreenMap2 = new CMap();
	m_cScreenMap2->Init(800, 1024, 25, 32, 800, 1024, 32);
	m_cScreenMap2->LoadMap("Image//Level_2-1.csv");
	m_cScreenMap2->scroll = false;
	setMap(m_cScreenMap2, enemyList, GoodiesList, BarrelsList, goList);
}

void CMap::InitScreenMap3(std::vector<CEnemy *> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList, std::vector<GameObject *> &goList)
{
	m_cScreenMap3 = new CMap();
	m_cScreenMap3->Init(800, 1024, 25, 32, 800, 1024, 32);
	m_cScreenMap3->LoadMap("Image//Level_3-1.csv");
	m_cScreenMap3->scroll = false;
	setMap(m_cScreenMap3, enemyList, GoodiesList, BarrelsList, goList);
}

void CMap::InitBossScrollingMap(vector<CEnemy*> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList, std::vector<CGoodies *> &HoleList, std::vector<GameObject *> &goList)
{
	m_cBossScrollingMap = new CMap();
	m_cBossScrollingMap->Init(800, 1024, 25, 32, 800, 2048, 32);
	m_cBossScrollingMap->LoadMap("Image//Level_Boss_Scrolling.csv");
	m_cBossScrollingMap->scroll = true;
	setMap(m_cBossScrollingMap, enemyList, GoodiesList, BarrelsList, goList);
}

void CMap::InitScrollingMap2(std::vector<CEnemy *> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList, std::vector<GameObject *> &goList)
{
	m_cScrollingMap2 = new CMap();
	m_cScrollingMap2->Init(800, 1024, 25, 32, 800, 2048, 32);
	m_cScrollingMap2->LoadMap("Image//Level_2-2.csv");
	m_cScrollingMap2->scroll = true;	
	setMap(m_cScrollingMap2, enemyList, GoodiesList, BarrelsList, goList);
}

void CMap::InitScrollingMap3(std::vector<CEnemy *> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList, std::vector<GameObject *> &goList)
{
	m_cScrollingMap3 = new CMap();
	m_cScrollingMap3->Init(800, 1024, 25, 32, 800, 2048, 32);
	m_cScrollingMap3->LoadMap("Image//Level_3-2.csv");
	m_cScrollingMap3->scroll = true;	
	setMap(m_cScrollingMap3, enemyList, GoodiesList, BarrelsList, goList);
}

void CMap::InitBossMap(vector<CEnemy*> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList, std::vector<CGoodies *> &HoleList, std::vector<GameObject *> &goList)
{
	m_cBossMap = new CMap();
	m_cBossMap->Init(800, 1024, 25, 32, 800, 1024, 32);
	m_cBossMap->LoadMap("Image//Level_Boss.csv");
	m_cBossMap->scroll = false;
	setMap(m_cBossMap, enemyList, GoodiesList, BarrelsList, goList);
}

void CMap::InitShopMap(vector<CEnemy*> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList, std::vector<GameObject *> &goList)
{
	m_cShopMap = new CMap();
	m_cShopMap->Init(800, 1024, 25, 32, 800, 1024, 32);
	m_cShopMap->LoadMap("Image//Level_Shop.csv");
	m_cShopMap->scroll = false;
	setMap(m_cShopMap, enemyList, GoodiesList, BarrelsList, goList);
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

void CMap::setMap(CMap* currMap, vector<CEnemy*> &enemyList, std::vector<CGoodies *> &GoodiesList, std::vector<CGoodies *> &BarrelsList, std::vector<GameObject *> &goList)
{
	int tempType;
	CEnemy* tempEnemy;
	CGoodies* tempGoodies;

	for(int i = 0; i < currMap->getNumOfTiles_MapHeight(); ++i)
	{
		for(int j = 0; j < currMap->getNumOfTiles_MapWidth(); ++j)
		{
			tempType = currMap->theScreenMap[i][j];

			if(tempType == CMap::JEWEL)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * currMap->GetTileSize(), currMap->GetTileSize() * (currMap->GetNumOfTiles_Height() - i) -  currMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::KEY)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * currMap->GetTileSize(), currMap->GetTileSize() * (currMap->GetNumOfTiles_Height() - i) -  currMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::KEY;	
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::CHEST)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * currMap->GetTileSize(), currMap->GetTileSize() * (currMap->GetNumOfTiles_Height() - i) -  currMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::CHEST;	
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);

				GameObject* go = new GameObject(GameObject::GO_PILLAR);
				goList.push_back(go);
				go->active = true;
				go->pos.Set(tempGoodies->GetPos_x() + 16,tempGoodies->GetPos_y() + 16,0);
				go->scale.Set(16,16,16);
				go->ID = tempType;
			}

			else if(tempType == CMap::HAY)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * currMap->GetTileSize(), currMap->GetTileSize() * (currMap->GetNumOfTiles_Height() - i) -  currMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::HAY;	
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::BARREL)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * currMap->GetTileSize(), currMap->GetTileSize() * (currMap->GetNumOfTiles_Height() - i) -  currMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::BARREL;	
				tempGoodies->tilePos.Set(j,i,0);
				BarrelsList.push_back(tempGoodies);
				GoodiesList.push_back(tempGoodies);

				GameObject* go = new GameObject(GameObject::GO_PILLAR);
				goList.push_back(go);
				go->active = true;
				go->pos.Set(tempGoodies->GetPos_x() + 16,tempGoodies->GetPos_y() + 16,0);
				go->scale.Set(16,16,16);
				go->ID = tempType;
			}

			else if(tempType == CMap::HPPOT)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * currMap->GetTileSize(), currMap->GetTileSize() * (currMap->GetNumOfTiles_Height() - i) - currMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::HPPOT;	
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::MAXHP)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * currMap->GetTileSize(), currMap->GetTileSize() * (currMap->GetNumOfTiles_Height() - i) - currMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::MAXHP;	
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::SCROLL)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * currMap->GetTileSize(), currMap->GetTileSize() * (currMap->GetNumOfTiles_Height() - i) - currMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::SCROLL;	
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			else if(tempType == CMap::HOLE)
			{
				tempGoodies = new CGoodies();
				tempGoodies->SetPos(j * currMap->GetTileSize(), currMap->GetTileSize() * (currMap->GetNumOfTiles_Height() - i) - currMap->GetTileSize());
				tempGoodies->active = true;
				tempGoodies->GoodiesType = CGoodies::Goodies_Type::HOLE;	
				tempGoodies->tilePos.Set(j,i,0);
				GoodiesList.push_back(tempGoodies);
			}

			else if((tempType >= CMap::ENEMY_50 && tempType < CMap::ENEMYAMOURED_80) || tempType >= ENEMYRANGED_100)
			{
				tempEnemy = new CEnemy();
				tempEnemy->ChangeStrategy(NULL,false);
				tempEnemy->SetPos_x(j * currMap->GetTileSize());
				tempEnemy->SetPos_y(currMap->GetTileSize() * (currMap->GetNumOfTiles_Height() - i) - currMap->GetTileSize());
				tempEnemy->active = true;
				tempEnemy->ID = tempType;
				tempEnemy->health = 2;
				tempEnemy->setWayPoints(currMap);
				tempEnemy->eneCurrTile = Vector3(j,i,0);
				enemyList.push_back(tempEnemy);

				GameObject* go = new GameObject(GameObject::GO_PILLAR);
				goList.push_back(go);
				go->active = true;
				go->pos.Set(tempEnemy->GetPos_x(),tempEnemy->GetPos_y(),0);
				go->scale.Set(16,16,16);
				go->ID = tempType;
			}

			else if(tempType >= CMap::ENEMYAMOURED_80 && tempType < ENEMYRANGED_100)
			{
				tempEnemy = new CEnemy();
				tempEnemy->ChangeStrategy(NULL,false);
				tempEnemy->SetPos_x(j * currMap->GetTileSize());
				tempEnemy->SetPos_y(currMap->GetTileSize() * (currMap->GetNumOfTiles_Height() - i) - currMap->GetTileSize());
				tempEnemy->active = true;
				tempEnemy->ID = tempType;
				tempEnemy->health = 3;
				tempEnemy->setWayPoints(currMap);
				tempEnemy->eneCurrTile = Vector3(j,i,0);	
				enemyList.push_back(tempEnemy);

				GameObject* go = new GameObject(GameObject::GO_PILLAR);
				goList.push_back(go);
				go->active = true;
				go->pos.Set(tempEnemy->GetPos_x(),tempEnemy->GetPos_y(),0);
				go->scale.Set(16,16,16);
				go->ID = tempType;
			}

			else if((tempType >= 29 && tempType <= 42) || (tempType >= 44 && tempType <= 49))
			{
				GameObject* go = new GameObject(GameObject::GO_WALL);
				goList.push_back(go);
				go->active = true;
				go->pos.Set(j * currMap->GetTileSize() + 16, currMap->GetTileSize() * (currMap->GetNumOfTiles_Height() - i) -  currMap->GetTileSize() + 16);
				go->scale.Set(32,32,32);
				go->normal.Set(0,1,0);
				go->ID = tempType;
			}

			else if(tempType == CMap::DOOR)
			{
				GameObject* go = new GameObject(GameObject::GO_WALL);
				goList.push_back(go);
				go->active = true;
				go->pos.Set(j * currMap->GetTileSize() + 16, currMap->GetTileSize() * (currMap->GetNumOfTiles_Height() - i) -  currMap->GetTileSize() + 16);
				go->scale.Set(32,32,32);
				go->normal.Set(1,0,0);
				go->ID = tempType;
			}
		}
	}
}