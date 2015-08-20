#include "SceneText.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"

#include <sstream>
#include <irrKlang.h>
#pragma comment (lib, "irrKlang.lib")
using namespace irrklang;

static char CHAR_HEROKEY;
static bool BOOL_HEROJUMP;
static const float TILE_SIZE = 32;

ISoundEngine *Name	= createIrrKlangDevice(ESOD_AUTO_DETECT, ESEO_MULTI_THREADED | ESEO_LOAD_PLUGINS | ESEO_USE_3D_BUFFERS);

SceneText::SceneText()
	: BossPointer(NULL)
{
}

SceneText::~SceneText()
{
}

void SceneText::Init()
{
	//Screen background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	//Enable depth test
	glEnable(GL_DEPTH_TEST);
	
	//Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Load shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	
	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(1, 1, 0);
	lights[1].color.Set(1, 1, 0.5f);
	lights[1].power = 0.4f;
	
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);
	
	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	
	// ================================= LOAD MESHES =================================

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//calibri.tga");
	
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 10.f);

	// ============================== Load Map tiles (Screen & scrolling) =============================

	meshList[GEO_SCREENTILESHEET] = MeshBuilder::GenerateTileMap("GEO_S_TILEGROUND", 5, 5);
	meshList[GEO_SCREENTILESHEET]->textureID = LoadTGA("Image//tilesheet.tga");

	meshList[GEO_TILEBACKGROUND] = MeshBuilder::Generate2DMesh("GEO_S_TILEGROUND", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILEBACKGROUND]->textureID = LoadTGA("Image//tile0_blank.tga");

	meshList[GEO_TILEHEROSHEET] = MeshBuilder::GenerateSprites("GEO_TILEHEROSHEET", 4, 4);
	meshList[GEO_TILEHEROSHEET]->textureID = LoadTGA("Image//Hero//hero.tga");

	meshList[GEO_TILEHEROSHEET2] = MeshBuilder::GenerateSprites("GEO_TILEHEROSHEET2", 2, 2);
	meshList[GEO_TILEHEROSHEET2]->textureID = LoadTGA("Image//Hero//hero2.tga");

	meshList[GEO_TILE] = MeshBuilder::GenerateTileMap("GEO_TILE", 6, 6);
	meshList[GEO_TILE]->textureID = LoadTGA("Image//tile.tga");
	// ================================= Load Enemies =================================
	
	meshList[GEO_TILEENEMY_FRAME0] = MeshBuilder::Generate2DMesh("GEO_TILEENEMY_FRAME0", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILEENEMY_FRAME0]->textureID = LoadTGA("Image//Enemy//tile20_enemy.tga");

	meshList[GEO_TILEENEMY_FRAME1] = MeshBuilder::Generate2DMesh("GEO_TILEENEMY_FRAME1", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILEENEMY_FRAME1]->textureID = LoadTGA("Image//Enemy//tile21_enemy.tga");

	// ==================================== Goodies ====================================

	meshList[GEO_DIAMOND] = MeshBuilder::Generate2DMesh("GEO_DIAMOND", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_DIAMOND]->textureID = LoadTGA("Image//Goodies//diamond.tga");

	meshList[GEO_KEY] = MeshBuilder::Generate2DMesh("GEO_KEY", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_KEY]->textureID = LoadTGA("Image//Goodies//key.tga");

	meshList[GEO_CHEST] = MeshBuilder::Generate2DMesh("GEO_CHEST", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_CHEST]->textureID = LoadTGA("Image//Goodies//chest.tga");

	meshList[GEO_TILE_WAYPOINT] = MeshBuilder::Generate2DMesh("GEO_TILE_WAYPOINT", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILE_WAYPOINT]->textureID = LoadTGA("Image//tile0_blank_red.tga");

	// ================================= Load Boss =================================

	meshList[GEO_TILEBOSS_FRAME0] = MeshBuilder::GenerateSprites("GEO_TILEENEMY_FRAME0", 3, 3);
	meshList[GEO_TILEBOSS_FRAME0]->textureID = LoadTGA("Image//Enemy//boss.tga");


	//Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(perspective);

	// === Set hero's position ===
	hero.settheHeroPositionx(920);
	hero.settheHeroPositiony(640);

	int tempHeroPosX = (int) ceil ((float)(920) / 32);
	int tempHeroPosY = 25 - (int) ceil ((float)(640 + 32) / 32);
	
	hero.heroCurrTile = Vector3(tempHeroPosX,tempHeroPosY,0);

	//Variables
	rotateAngle = 0;

	//Game variables
	level = 1;
	attackSpeed = 0;
	DistanceFromEnemyX = 0;
	DistanceFromEnemyY = 0;

	//Sound effects

	// Sprites Variable
	heroTileID = 0;

	if(level == 1)
	{
		// === Initialise and Load the Screenmap ===
		map.InitScreenMap(enemyList, GoodiesList);
		CurrentMap = map.m_cScreenMap;
	}

	else if(level == 2)
	{
		// === Initialise and load the tilemap ===
		map.InitMap(enemyList, GoodiesList);
		CurrentMap = map.m_cMap;
	}

	else if (level == 7)
	{
		map.InitBossMap(enemyList, GoodiesList);
		CurrentMap = map.m_cBossMap;
	}

	// === Boss's Variables and Pointers ===
	BossPointer = new CBoss();
	BossPointer->BossInit();

	BossTileID = 0;
	bossCounter = 0.0f;
	IsTurn = false;
	EnemiesRendered = false;
}

void SceneText::Update(double dt)
{
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(Application::IsKeyPressed('5'))
	{
		lights[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	
	else if(Application::IsKeyPressed('6'))
	{
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	
	else if(Application::IsKeyPressed('7'))
	{
		lights[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	
	else if(Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	
	else if(Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}

	// =================================== UPDATE THE HERO ===================================
	
	if(Application::IsKeyPressed('A'))
	{
		CHAR_HEROKEY = 'a';
	
		//Sprite Animation
		heroTileID += 0.1;
		if(heroTileID < 4)
		{
			heroTileID = 4;
		}
		
		if(heroTileID > 6)
		{
			heroTileID = 4;
		}
	}

	else if(Application::IsKeyPressed('D'))
	{
		CHAR_HEROKEY = 'd';

		//Sprite Animation
		heroTileID += 0.1;
		if(heroTileID < 8)
		{
			heroTileID = 8;
		}
		
		if(heroTileID > 10)
		{
			heroTileID = 8;
		}
	}

	else if(Application::IsKeyPressed('W'))
	{
		CHAR_HEROKEY = 'w';
		
		//Sprite Animation
		heroTileID += 0.1;
		if(heroTileID < 12)
		{
			heroTileID = 12;
		}
		
		if(heroTileID > 14)
		{
			heroTileID = 12;
		}
	}

	else if(Application::IsKeyPressed('S'))
	{
		CHAR_HEROKEY = 's';

		//Sprite Animation
		heroTileID += 0.1;
		if(heroTileID < 0)
		{
			heroTileID = 0;
		}
		
		if(heroTileID > 2)
		{
			heroTileID = 0;
		}
	}

	//Limit hero's attak rate
	if(hero.GetAttackStatus() == true)
	{
		attackSpeed += dt;
		if(attackSpeed >= 0.5)
		{
			attackSpeed = 0;
			hero.SetAttackStatus(false);
		}
	}

	//Get dagger
	if(hero.GetPickUpWeapon() == true && Application::IsKeyPressed(VK_SPACE))
	{
		hero.SetDaggerAcquired(true);
	}

	// =================================== UPDATE THE ENEMY ===================================

	for(std::vector<CEnemy *>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		CEnemy *go = (CEnemy *)*it;
		if(go->active)	
		{
			if(go->currentStrat != CEnemy::STRAT_KILL)
			{
				go->ChangeStrategy(new CStrategy_Kill());
				go->currentStrat = CEnemy::STRAT_KILL;
			}

			go->SetDestination(hero.gettheHeroPositionx() + CurrentMap->mapOffset_x, hero.gettheHeroPositiony());
			go->Update(CurrentMap, hero.heroCurrTile);

			DistanceFromEnemyX = hero.gettheHeroPositionx() - go->GetPos_x() +  CurrentMap->mapOffset_x;
			DistanceFromEnemyY = hero.gettheHeroPositiony() - go->GetPos_y();
			CheckEnemiesInRange(go);
		}

	}
	
	// =================================== UPDATE THE GOODIES ===================================
	
	for(std::vector<CGoodies *>::iterator it = GoodiesList.begin(); it != GoodiesList.end(); ++it)
	{
		CGoodies *go = (CGoodies *)*it;
		if(go->active)	
		{
			if(go->CalculateDistance(hero.gettheHeroPositionx() + CurrentMap->mapOffset_x, hero.gettheHeroPositiony()) == true)
			{
				if(go->GoodiesType != CGoodies::Goodies_Type::DOOR)
				{
					go->active = false;
					if(go->GoodiesType == CGoodies::Goodies_Type::KEY)
					{
						hero.SetKeyAcquired(true);
					
					}
				}
			}
		}
	}


	// =================================== BOSS UPDATES ===================================
	BossPointer->Set_BossDestination(BossPointer->Get_BossX(), BossPointer->Get_BossY());

	bossCounter += 0.01f;

	if (bossCounter < 2.0f)
	{
		BossTileID++;
		if (BossTileID > 2)
		{
			BossTileID = 0;
			IsTurn = false;
		}
	}
	else if (bossCounter > 2.0f && bossCounter < 4.0f)
	{
		BossTileID++;
		if (BossTileID > 5)
		{
			BossTileID = 3;
			IsTurn = true;

			for (int i = 0; i < CurrentMap->GetNumOfTiles_Height(); i++)
			{
				for (int k = 0; k < CurrentMap->GetNumOfTiles_Width() + 1; k++)
				{
					if (CurrentMap->theScreenMap[CurrentMap->GetNumOfTiles_Height() - (hero.gettheHeroPositiony() / 32)][hero.gettheHeroPositionx() / 32] == 0)
					{
						EnemiesRendered = true;
					}
				}
			}
		}
	}
	else if (bossCounter > 4.0f)
	{
		bossCounter = 0;
	}


	BossPointer->Set_SpawnGuards(IsTurn);

	// =================================== MAIN UPDATES ===================================

	hero.HeroUpdate(CurrentMap, CHAR_HEROKEY, BOOL_HEROJUMP, level);
	CHAR_HEROKEY = NULL;

	//map traversing aka character can move from 1 map to another 
	int checkPosition_X = (int)((CurrentMap->mapOffset_x + hero.gettheHeroPositionx()) /CurrentMap->GetTileSize());
	int checkPosition_Y = CurrentMap->GetNumOfTiles_Height() - (int)((hero.gettheHeroPositiony() + CurrentMap->GetTileSize()) / CurrentMap->GetTileSize());
	
	//Moving from screen stage to scrollnig stage conditions
	if(CurrentMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == CMap::DOOR)
	{
		if(hero.GetKeyAcquired() == true)
		{
			if(hero.GetdoorOpened() == false)
			{
				if(Application::IsKeyPressed(VK_SPACE))
				{
					hero.SetdoorOpened(true);
					for(std::vector<CGoodies *>::iterator it = GoodiesList.begin(); it != GoodiesList.end(); ++it)
					{
						CGoodies *go = (CGoodies *)*it;
						if(go->active)	
						{	
							if(go->GoodiesType == CGoodies::Goodies_Type::DOOR)
							{
								go->active = false;
							}
						}
					}
				}
			}
		}
	}
	
	if(CurrentMap->theScreenMap[checkPosition_Y][checkPosition_X] == CMap::DOOR)
	{
		if(hero.GetdoorOpened() == true)
		{
			if(level == 1)
			{
				hero.SetKeyAcquired(false);	
				hero.SetdoorOpened(false);
				level = 2;
				hero.settheHeroPositionx(32);
				hero.heroCurrTile.x = 1;
				enemyList.erase(enemyList.begin(), enemyList.end());
				GoodiesList.erase(GoodiesList.begin(), GoodiesList.end());
				map.InitMap(enemyList, GoodiesList);
				CurrentMap = map.m_cMap;
			}

			else if(level == 2)
			{
				level = 1;
				hero.settheHeroPositionx(1024 - 32);
				enemyList.erase(enemyList.begin(), enemyList.end());
				GoodiesList.erase(GoodiesList.begin(), GoodiesList.end());
				map.InitScreenMap(enemyList, GoodiesList);
				CurrentMap = map.m_cScreenMap;	
			}

			else if (level == 7)
			{
				hero.settheHeroPositionx(32);
				enemyList.erase(enemyList.begin(), enemyList.end());
				GoodiesList.erase(GoodiesList.begin(), GoodiesList.end());
				map.InitMap(enemyList, GoodiesList);
				CurrentMap = map.m_cBossMap;
			}
		}
	}

	camera.Update(dt);
	fps = (float)(1.f / dt);
}

void SceneText::UpdateCameraStatus(const unsigned char key, const bool status)
{
}

void SceneText::UpdateAttackStatus(const unsigned char key)
{
	if(key == CA_ATTACK && hero.GetDaggerAcquired() == true)
	{
		hero.SetAttackStatus(true);
	}
}

void SceneText::CheckEnemiesInRange(CEnemy *go)
{
	if(hero.GetAttackStatus() == true)
	{
		//Check enemies in x-order
		if(DistanceFromEnemyY == 0)
		{
			if(DistanceFromEnemyX >= 15 && DistanceFromEnemyX <= 30)
			{
				if(hero.GetAnimationInvert() == true)
				{
					go->active = false;
				}
			}

			else if(DistanceFromEnemyX <= -15 && DistanceFromEnemyX >= -30)
			{
				if(hero.GetAnimationInvert() == false)
				{
					go->active = false;
				}
			}
		}

		//Check enemies in Y-order
		else if(DistanceFromEnemyX == 0)
		{
			if(DistanceFromEnemyY >= 15 && DistanceFromEnemyY <= 30)
			{
				if(hero.GetAnimationFlip() == false)
				{
					go->active = false;
				}
			}

			else if(DistanceFromEnemyY <= -15 && DistanceFromEnemyY >= -30)
			{
				if(hero.GetAnimationFlip() == true)
				{
					go->active = false;
				}
			}
		}
	}
}

void SceneText::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneText::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(size, size, size);
				glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
				glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
				glUniform1i(m_parameters[U_LIGHTENABLED], 0);
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh->textureID);
				glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				
				for(unsigned i = 0; i < text.length(); ++i)
				{
					Mtx44 characterSpacing;
					characterSpacing.SetToTranslation(i * 0.6f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
					Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
					glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
					mesh->Render((unsigned)text[i] * 6, 6);
				}
				
				glBindTexture(GL_TEXTURE_2D, 0);
				glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneText::RenderMeshIn2D(Mesh *mesh, const bool enableLight, const float size, const float x, const float y, const bool rotate)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(size, size, size);
				
				if(rotate)
					modelStack.Rotate(rotateAngle, 0, 0, 1);
       
				Mtx44 MVP, modelView, modelView_inverse_transpose;
	
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				
				if(mesh->textureID > 0)
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mesh->textureID);
					glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				}
				
				else
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
				}
				
				mesh->Render();
				
				if(mesh->textureID > 0)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}
       
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneText::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
		
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	
	mesh->Render();
	
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneText::Render2DMesh(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 1024, 0, 800, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(size, size, size);
				
				if(rotate)
				{
					//glDisable(GL_CULL_FACE);
					//modelStack.Rotate(180, 0, 1, 0);
				}
       
				Mtx44 MVP, modelView, modelView_inverse_transpose;
	
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				
				if(mesh->textureID > 0)
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mesh->textureID);
					glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				}
				
				else
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
				}
				
				mesh->Render();
				
				if(mesh->textureID > 0)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}
       
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneText::RenderQuadOnScreen(Mesh* mesh, float sizeX, float sizeY, float x, float y, bool enableLight)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);	//size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();					//No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();					//Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizeX, sizeY, 0);


	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	if(enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}

	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}

	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	mesh->Render();

	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void SceneText::RenderSprites(Mesh* mesh, int id, const float size, const float x, const float y)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 1024, 0, 800, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

	//if (!mesh || mesh->textureID <= 0)
	//	return;

	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	Mtx44 characterSpacing;
	characterSpacing.SetToTranslation(0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	mesh->Render((unsigned)id * 6, 6);

	glBindTexture(GL_TEXTURE_2D, 0);

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneText::RenderTilesMap(Mesh* mesh, int ID, const float size, const float x, const float y)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 1024, 0, 800, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

	//if (!mesh || mesh->textureID <= 0)
	//	return;

	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	Mtx44 characterSpacing;
	characterSpacing.SetToTranslation(0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	mesh->Render((unsigned)ID * 6, 6);

	glBindTexture(GL_TEXTURE_2D, 0);

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneText::RenderInit()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(perspective);
	
	//Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	
	//Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
}

void SceneText::RenderText()
{
	//On screen text
	std::ostringstream ss;
	ss.precision(4);
	ss << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 2.3, 2, 1);
	
	std::ostringstream ss1;
	ss1.precision(5);
	ss1 << "Position: " << hero.gettheHeroPositionx() << "," << hero.gettheHeroPositiony();
	RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(1, 1, 0), 2.3, 2, 57);

	std::ostringstream ss2;
	ss2.precision(5);
	ss2 << "MapOffset_x: " << CurrentMap->mapOffset_x;
	RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(1, 1, 0), 2.3, 2, 53);

	std::ostringstream ss3;
	ss3.precision(5);
	ss3 << "TileOffset_x:" <<CurrentMap->tileOffset_x;	
	RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(1, 1, 0), 2.3, 2, 49);
}

void SceneText::RenderHero()
{
	if(hero.GetAttackStatus() == true)
	{
		if((heroTileID >= 0 && heroTileID <= 2) && hero.GetAnimationFlip() == false)
		{
			RenderSprites(meshList[GEO_TILEHEROSHEET2], 0, 32, hero.gettheHeroPositionx(), hero.gettheHeroPositiony());
		}

		else if((heroTileID >= 4 && heroTileID <= 6) && hero.GetAnimationInvert() == true)
		{
			RenderSprites(meshList[GEO_TILEHEROSHEET2], 1, 32, hero.gettheHeroPositionx(), hero.gettheHeroPositiony());
		}

		else if((heroTileID >= 8 && heroTileID <= 10) && hero.GetAnimationInvert() == false)
		{
			RenderSprites(meshList[GEO_TILEHEROSHEET2], 2, 32, hero.gettheHeroPositionx(), hero.gettheHeroPositiony());
		}

		else
		{
			RenderSprites(meshList[GEO_TILEHEROSHEET2], 3, 32, hero.gettheHeroPositionx(), hero.gettheHeroPositiony());
		}
	}

	else
	{
		//Walking
		RenderSprites(meshList[GEO_TILEHEROSHEET], heroTileID, 32, hero.gettheHeroPositionx(), hero.gettheHeroPositiony());
	}
}

void SceneText::RenderEnemies()
{
	for(vector<CEnemy *>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		CEnemy *go = (CEnemy *)*it;
		int theEnemy_x = go->GetPos_x() - CurrentMap->mapOffset_x;
		int theEnemy_y = go->GetPos_y();
		
		if(go->active)	
		{			
			Render2DMesh(meshList[GEO_TILEENEMY_FRAME0], false, 1.0f, theEnemy_x, theEnemy_y);	
		}

		else
		{
			Render2DMesh(meshList[GEO_TILEENEMY_FRAME1], false, 1.0f, theEnemy_x, theEnemy_y);
		}
	}
}

void SceneText::RenderTileMap()
{
	int m = 0;	
	CurrentMap->mapFineOffset_x = CurrentMap->mapOffset_x % CurrentMap->GetTileSize();
	
	for(int i = 0; i < CurrentMap->GetNumOfTiles_Height(); i++)
	{
		for(int k = 0; k < CurrentMap->GetNumOfTiles_Width() + 1; k++)
		{
			m = CurrentMap->tileOffset_x + k;

			//If we have reached the right side of the map, then do not display the extra column of tiles
			if(m >= CurrentMap->getNumOfTiles_MapWidth())
			{
				break;
			}
			
			if(level == 1)
			{
				if (CurrentMap->theScreenMap[i][m] < 0)
				{
					Render2DMesh(meshList[GEO_TILE_WAYPOINT], false, 1.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x, 768 - i * CurrentMap->GetTileSize());
				}

				else if(CurrentMap->theScreenMap[i][m] != CMap::WALL && CurrentMap->theScreenMap[i][m] != CMap::DOOR)
				{
					Render2DMesh(meshList[GEO_TILEBACKGROUND], false, 1.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x, 768 - i * CurrentMap->GetTileSize());
				}

				else if(CurrentMap->theScreenMap[i][m] >= 1)
				{
					if(CurrentMap->theScreenMap[i][m] != CMap::DOOR)
					{
						RenderTilesMap(meshList[GEO_SCREENTILESHEET], CurrentMap->theScreenMap[i][m], 32.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x, 768 - i * CurrentMap->GetTileSize());
					}
					else
					{
						if(hero.GetdoorOpened() == false)
						{
							RenderTilesMap(meshList[GEO_SCREENTILESHEET], CurrentMap->theScreenMap[i][m], 32.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x, 768 - i * CurrentMap->GetTileSize());
						}
						else
						{
							Render2DMesh(meshList[GEO_TILEBACKGROUND], false, 1.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x, 768 - i * CurrentMap->GetTileSize());
						}
						

					}
												
					
				}
			}
			
			else if(level == 2)
			{
				if(CurrentMap->theScreenMap[i][m] != CMap::WALL && CurrentMap->theScreenMap[i][m] != CMap::DOOR)
				{
					Render2DMesh(meshList[GEO_TILEBACKGROUND], false, 1.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x, 768 - i * CurrentMap->GetTileSize());
				}

				else if(CurrentMap->theScreenMap[i][m] >= 1)
				{
					RenderTilesMap(meshList[GEO_SCREENTILESHEET],CurrentMap->theScreenMap[i][m], 32.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x, 768 - i * CurrentMap->GetTileSize());
				}
			}

			else if (level == 7)
			{
				int m = 0;
				CurrentMap->mapFineOffset_x = CurrentMap->mapOffset_x % CurrentMap->GetTileSize();

				m = CurrentMap->tileOffset_x + k;

				//If we have reached the right side of the map, then do not display the extra column of tiles
				if (m >= CurrentMap->getNumOfTiles_MapWidth())
				{
					break;
				}
				if (CurrentMap->theScreenMap[i][m] >= 0)
				{
					RenderTilesMap(meshList[GEO_TILE], CurrentMap->theScreenMap[i][m], 32.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x, 768 - i * CurrentMap->GetTileSize());
				}

				if (CurrentMap->theScreenMap[CurrentMap->GetNumOfTiles_Height() - (hero.gettheHeroPositiony() / 32)][hero.gettheHeroPositionx() / 32] == 0 && IsTurn == true || EnemiesRendered == true)
				{
					BossPointer->BossState = CBoss::B_SPAWN;

					if (BossPointer->BossState == CBoss::B_SPAWN)
					{
						if (CurrentMap->theScreenMap[i][m] == 6)
						{
							Render2DMesh(meshList[GEO_TILEENEMY_FRAME0], false, 1.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x - 32, 768 - i * CurrentMap->GetTileSize());
						}
					}
				}

				RenderSprites(meshList[GEO_TILEBOSS_FRAME0], BossTileID, 32, 45, 400);
			}
		}
	}
}

void SceneText::RenderGoodies()
{
	//Render the goodies
	for(vector<CGoodies *>::iterator it = GoodiesList.begin(); it != GoodiesList.end(); ++it)
	{
		CGoodies *go = (CGoodies *)*it;
		if(go->active)	
		{
			int theGoodies_x = go->GetPos_x() - map.mapOffset_x;
			int theGoodies_y = go->GetPos_y();
			
			if(go->GoodiesType == CGoodies::Goodies_Type::JEWEL)
			{
				Render2DMesh(meshList[GEO_DIAMOND], false, 1.0f,theGoodies_x -  CurrentMap->mapOffset_x, theGoodies_y);	
			}
			
			else if(go->GoodiesType == CGoodies::Goodies_Type::KEY)
			{
				Render2DMesh(meshList[GEO_KEY], false, 1.0f, theGoodies_x - CurrentMap->mapOffset_x, theGoodies_y);	
			}
			
			else if(go->GoodiesType == CGoodies::Goodies_Type::CHEST)
			{
				Render2DMesh(meshList[GEO_CHEST], false, 1.0f, theGoodies_x - CurrentMap->mapOffset_x, theGoodies_y);	
			}
		}
	}
}

void SceneText::Render()
{
	RenderInit();
	RenderTileMap();
	RenderEnemies();
	RenderGoodies();
	RenderHero();
	RenderText();
}

void SceneText::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}