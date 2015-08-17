#include "Map.h"

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
, m_cMap(NULL)
, m_cScreenMap(NULL)
{
	theScreenMap.clear();
}

CMap::~CMap(void)
{
	if(m_cMap)
	{
		delete m_cMap;
		m_cMap = NULL;
	}

	if(m_cScreenMap)
	{
		delete m_cScreenMap;
		m_cScreenMap = NULL;
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

void CMap::InitMap()
{
	m_cMap = new CMap();
	m_cMap->Init(800, 1024, 25, 32, 800, 2048, 32);
	m_cMap->LoadMap("Image//MapDesign_Scrolling.csv");
}


void CMap::InitScreenMap(std::vector<CEnemy *> &enemyList)
{
	m_cScreenMap = new CMap();
	m_cScreenMap->Init(800, 1024, 25, 32, 800, 1024, 32);
	m_cScreenMap->LoadMap("Image//MapDesign_Screen.csv");

	int tempType;
	CEnemy* tempEnemy;

	for (int i = 0; i < m_cScreenMap->getNumOfTiles_MapHeight(); ++i)
	{
		for (int j = 0; j < m_cScreenMap->getNumOfTiles_MapWidth(); ++j)
		{
			tempType = m_cScreenMap->theScreenMap[i][j];

			switch (tempType)
			{
			case 50:
				tempEnemy = new CEnemy();
				tempEnemy->ChangeStrategy(NULL,false);
				tempEnemy->SetPos_x(j * m_cScreenMap->GetTileSize());
				tempEnemy->SetPos_y(m_cScreenMap->GetTileSize() * (m_cScreenMap->GetNumOfTiles_Height() - i) -  m_cScreenMap->GetTileSize());
				tempEnemy->active = true;
				enemyList.push_back(tempEnemy);
				break;
			default:
				break;
			}
		}
	}
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