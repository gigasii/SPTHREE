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
static const float TILE_SIZE = 32;

ISoundEngine *Name	= createIrrKlangDevice(ESOD_AUTO_DETECT, ESEO_MULTI_THREADED | ESEO_LOAD_PLUGINS | ESEO_USE_3D_BUFFERS);

SceneText::SceneText()
	: CurrentMap(NULL)
	, CustomMap(NULL)
	, BossPointer(NULL)
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
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//Font//calibri.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//Font//c.tga");

	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 10.f);

	// ============================== Load Map tiles (Screen & scrolling) =============================

	meshList[GEO_TILEBACKGROUND] = MeshBuilder::Generate2DMesh("GEO_S_TILEGROUND", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILEBACKGROUND]->textureID = LoadTGA("Image//ground.tga");

	meshList[GEO_TILEDOOR] = MeshBuilder::Generate2DMesh("GEO_WALL", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILEDOOR]->textureID = LoadTGA("Image//door.tga");
	
	meshList[GEO_TILE] = MeshBuilder::GenerateTileMap("GEO_TILE", 6, 6);
	meshList[GEO_TILE]->textureID = LoadTGA("Image//tile2.tga");

	meshList[GEO_TILEDETECTIONRADIUS] = MeshBuilder::Generate2DMesh("GEO_TILE_WAYPOINT", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_TILEDETECTIONRADIUS]->textureID = LoadTGA("Image//tile0_blank_red.tga");

	meshList[GEO_TILEEXCLAMATIONMARK] = MeshBuilder::Generate2DMesh("GEO_TILEEXCLAMATIONMARK", Color(1, 1, 1), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_TILEEXCLAMATIONMARK]->textureID = LoadTGA("Image//exclamationmark.tga");

	meshList[GEO_20] = MeshBuilder::Generate2DMesh("GEO_20", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_20]->textureID = LoadTGA("Image//Desert//tile20.tga");

	meshList[GEO_21] = MeshBuilder::Generate2DMesh("GEO_21", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_21]->textureID = LoadTGA("Image//Desert//tile21.tga");

	meshList[GEO_22] = MeshBuilder::Generate2DMesh("GEO_22", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_22]->textureID = LoadTGA("Image//Desert//tile22.tga");

	meshList[GEO_23] = MeshBuilder::Generate2DMesh("GEO_23", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_23]->textureID = LoadTGA("Image//Desert//tile23.tga");

	meshList[GEO_24] = MeshBuilder::Generate2DMesh("GEO_24", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_24]->textureID = LoadTGA("Image//Desert//tile24.tga");

	meshList[GEO_25] = MeshBuilder::Generate2DMesh("GEO_25", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_25]->textureID = LoadTGA("Image//Desert//tile25.tga");

	meshList[GEO_26] = MeshBuilder::Generate2DMesh("GEO_26", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_26]->textureID = LoadTGA("Image//Desert//tile26.tga");

	meshList[GEO_27] = MeshBuilder::Generate2DMesh("GEO_27", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_27]->textureID = LoadTGA("Image//Desert//tile27.tga");

	meshList[GEO_28] = MeshBuilder::Generate2DMesh("GEO_28", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_28]->textureID = LoadTGA("Image//Desert//tile28.tga");

	meshList[GEO_29] = MeshBuilder::Generate2DMesh("GEO_29", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_29]->textureID = LoadTGA("Image//Desert//tile29.tga");

	meshList[GEO_30] = MeshBuilder::Generate2DMesh("GEO_30", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_30]->textureID = LoadTGA("Image//Desert//tile30.tga");

	meshList[GEO_31] = MeshBuilder::Generate2DMesh("GEO_31", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_31]->textureID = LoadTGA("Image//Desert//tile31.tga");

	meshList[GEO_32] = MeshBuilder::Generate2DMesh("GEO_32", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_32]->textureID = LoadTGA("Image//Desert//tile32.tga");

	meshList[GEO_33] = MeshBuilder::Generate2DMesh("GEO_33", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_33]->textureID = LoadTGA("Image//Desert//tile33.tga");

	meshList[GEO_34] = MeshBuilder::Generate2DMesh("GEO_34", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_34]->textureID = LoadTGA("Image//Desert//tile34.tga");

	meshList[GEO_35] = MeshBuilder::Generate2DMesh("GEO_35", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_35]->textureID = LoadTGA("Image//Desert//tile35.tga");

	meshList[GEO_36] = MeshBuilder::Generate2DMesh("GEO_36", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_36]->textureID = LoadTGA("Image//Desert//tile36.tga");

	meshList[GEO_37] = MeshBuilder::Generate2DMesh("GEO_37", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_37]->textureID = LoadTGA("Image//Desert//tile37.tga");

	meshList[GEO_38] = MeshBuilder::Generate2DMesh("GEO_38", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_38]->textureID = LoadTGA("Image//Desert//tile38.tga");

	meshList[GEO_39] = MeshBuilder::Generate2DMesh("GEO_39", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_39]->textureID = LoadTGA("Image//Desert//tile39.tga");

	meshList[GEO_40] = MeshBuilder::Generate2DMesh("GEO_40", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_40]->textureID = LoadTGA("Image//Desert//tile40.tga");

	meshList[GEO_41] = MeshBuilder::Generate2DMesh("GEO_41", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_41]->textureID = LoadTGA("Image//Desert//tile41.tga");

	meshList[GEO_42] = MeshBuilder::Generate2DMesh("GEO_42", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_42]->textureID = LoadTGA("Image//Desert//tile42.tga");

	meshList[GEO_43] = MeshBuilder::Generate2DMesh("GEO_43", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_43]->textureID = LoadTGA("Image//Desert//tile43.tga");

	meshList[GEO_44] = MeshBuilder::Generate2DMesh("GEO_44", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_44]->textureID = LoadTGA("Image//Desert//tile44.tga");

	meshList[GEO_45] = MeshBuilder::Generate2DMesh("GEO_45", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_45]->textureID = LoadTGA("Image//Desert//tile45.tga");

	meshList[GEO_46] = MeshBuilder::Generate2DMesh("GEO_46", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_46]->textureID = LoadTGA("Image//Desert//tile46.tga");

	meshList[GEO_47] = MeshBuilder::Generate2DMesh("GEO_47", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_47]->textureID = LoadTGA("Image//Desert//tile47.tga");

	meshList[GEO_48] = MeshBuilder::Generate2DMesh("GEO_48", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_48]->textureID = LoadTGA("Image//Desert//tile48.tga");

	meshList[GEO_49] = MeshBuilder::Generate2DMesh("GEO_49", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_49]->textureID = LoadTGA("Image//Desert//tile49.tga");

	// =================================== Load Hero ===================================

	meshList[GEO_TILEHEROSHEET] = MeshBuilder::GenerateSprites("GEO_TILEHEROSHEET", 4, 4);
	meshList[GEO_TILEHEROSHEET]->textureID = LoadTGA("Image//Hero//hero.tga");

	// ================================= Load Enemies =================================

	meshList[GEO_TILEENEMYSHEET] = MeshBuilder::GenerateSprites("GEO_TILEENEMYSHEET", 5, 5);
	meshList[GEO_TILEENEMYSHEET]->textureID = LoadTGA("Image//Enemy//enemy.tga");

	// ==================================== Goodies ====================================

	meshList[GEO_DIAMOND] = MeshBuilder::Generate2DMesh("GEO_DIAMOND", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_DIAMOND]->textureID = LoadTGA("Image//Goodies//diamond.tga");

	meshList[GEO_KEY] = MeshBuilder::Generate2DMesh("GEO_KEY", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_KEY]->textureID = LoadTGA("Image//Goodies//key.tga");

	meshList[GEO_CHEST] = MeshBuilder::Generate2DMesh("GEO_CHEST", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_CHEST]->textureID = LoadTGA("Image//Goodies//chest.tga");

	meshList[GEO_CHEST_OPENED] = MeshBuilder::Generate2DMesh("GEO_CHEST_OPENED", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_CHEST_OPENED]->textureID = LoadTGA("Image//Goodies//chest_opened.tga");

	meshList[GEO_BARREL] = MeshBuilder::Generate2DMesh("GEO_BARREL", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_BARREL]->textureID = LoadTGA("Image//Goodies//barrel.tga");

	meshList[GEO_BARREL_BROKEN] = MeshBuilder::Generate2DMesh("GEO_BARREL_BROKEN", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_BARREL_BROKEN]->textureID = LoadTGA("Image//Goodies//barrel_broken.tga");

	meshList[GEO_HAY] = MeshBuilder::Generate2DMesh("GEO_HAY", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_HAY]->textureID = LoadTGA("Image//Goodies//hay2.tga");

	meshList[GEO_HOLE] = MeshBuilder::Generate2DMesh("GEO_HOLE", Color(1, 1, 1), 0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
	meshList[GEO_HOLE]->textureID = LoadTGA("Image//Goodies//hole.tga");
	// ================================= Load Boss =================================

	meshList[GEO_TILEBOSS_FRAME0] = MeshBuilder::GenerateSprites("GEO_TILEENEMY_FRAME0", 3, 3);
	meshList[GEO_TILEBOSS_FRAME0]->textureID = LoadTGA("Image//Enemy//boss.tga");

	// ================================= Load Menu =================================

	meshList[GEO_MENU] = MeshBuilder::GenerateQuad("menu", Color(1, 1, 1), 1);
	meshList[GEO_MENU]->textureID = LoadTGA("Image//menu.tga");

	// ================================= Load Game Over Screen =================================
	meshList[GEO_LOSE] = MeshBuilder::GenerateQuad("gameover", Color(1, 1, 1), 1);
	meshList[GEO_LOSE]->textureID = LoadTGA("Image//lose_screen.tga");

	// ==================================== Load HUD ====================================

	meshList[GEO_HUD_HEART] = MeshBuilder::Generate2DMesh("GEO_HUD_HEART", Color(1, 1, 1), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_HUD_HEART]->textureID = LoadTGA("Image//HUD//heart.tga");

	meshList[GEO_HUD_KEY] = MeshBuilder::Generate2DMesh("GEO_HUD_KEY", Color(1, 1, 1), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_HUD_KEY]->textureID = LoadTGA("Image//Goodies//key.tga");

	meshList[GEO_HUD_DIAMOND] = MeshBuilder::Generate2DMesh("GEO_HUD_DIAMOND", Color(1, 1, 1), 0.0f, 0.0f, 1.0f, 1.0f);
	meshList[GEO_HUD_DIAMOND]->textureID = LoadTGA("Image//HUD//diamond.tga");

	// ==================================================================================

	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(perspective);

	// === Set hero's position ===

	hero.settheHeroPositionx(888);
	hero.settheHeroPositiony(640);

	int tempHeroPosX = (int) ceil ((float)(888) / 32);
	int tempHeroPosY = 25 - (int) ceil ((float)(640 + 32) / 32);

	hero.heroCurrTile = Vector3(tempHeroPosX, tempHeroPosY, 0);

	// === Variables ===
	
	rotateAngle = 0;

	// === Game variables ===	
	
	stage = 7;
	attackSpeed = 0;
	stabOnce = false;

	// === Boss's Variables and Pointers ===

	BossPointer = new CBoss();
	BossPointer->BossInit();
	BossTileID = 0;
	bossCounter = 0.0f;
	IsTurn = false;
	EnemiesRendered = false;
	derenderDoor = false;
	GetKey = false;

	// === HUD Variables ===

	diamondCount = 0;
	keyCount = 0;
	PointSystem = 0;

	// === Menu Variables ===

	menu = true;
	InteractHighLight = 0;
	delay = 0;
	Text[0] = "Start Game";
	Text[1] = "How To Play?";

	// === Custom cMenu Variables ===
	RenderCustomMenu = false;
	LoseTimer = 0.0f;

	// ========================== Initializing Map Inits ==========================

	if(stage == 1)
	{
		map.InitScreenMap(enemyList, GoodiesList, BarrelList);
		CurrentMap = map.m_cScreenMap;
	}

	else if(stage == 2)
	{
		map.InitScrollingMap(enemyList, GoodiesList, BarrelList);
		CurrentMap = map.m_cScrollingMap;
	}

	else if(stage == 7)
	{
		map.InitBossMap(enemyList, GoodiesList, BarrelList, HoleList);
		CurrentMap = map.m_cBossMap;
	}

	if (stage >= 0)
	{
		map.InitCustomMap();
		CustomMap = map.m_cCustomMap;
	}
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
		hero.heroTileID += 0.1;
		if(hero.heroTileID < 4)
		{
			hero.heroTileID = 4;
		}

		if(hero.heroTileID > 6)
		{
			hero.heroTileID = 4;
		}
	}

	else if(Application::IsKeyPressed('D'))
	{
		CHAR_HEROKEY = 'd';

		//Sprite Animation
		hero.heroTileID += 0.1;
		if(hero.heroTileID < 8)
		{
			hero.heroTileID = 8;
		}

		if(hero.heroTileID > 10)
		{
			hero.heroTileID = 8;
		}
	}

	else if(Application::IsKeyPressed('W'))
	{
		CHAR_HEROKEY = 'w';

		//Sprite Animation
		hero.heroTileID += 0.1;
		if(hero.heroTileID < 12)
		{
			hero.heroTileID = 12;
		}

		if(hero.heroTileID > 14)
		{
			hero.heroTileID = 12;
		}
	}

	else if(Application::IsKeyPressed('S'))
	{
		CHAR_HEROKEY = 's';

		//Sprite Animation
		hero.heroTileID += 0.1;
		if(hero.heroTileID < 0)
		{
			hero.heroTileID = 0;
		}

		if(hero.heroTileID > 2)
		{
			hero.heroTileID = 0;
		}
	}

	//Limit hero's attak rate
	if(hero.GetAttackStatus() == true)
	{
		attackSpeed += dt;
		if(attackSpeed >= 0.7)
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
			go->theStrategy->CurrentState;

			if(go->currentStrat != CEnemy::STRAT_KILL)
			{
				go->ChangeStrategy(new CStrategy_Kill());
				go->currentStrat = CEnemy::STRAT_KILL;
			}

			go->SetDestination(hero.gettheHeroPositionx() + CurrentMap->mapOffset_x, hero.gettheHeroPositiony());
			go->Update(CurrentMap, hero.heroCurrTile, BarrelList);

			int DistanceFromEnemyX = hero.gettheHeroPositionx() - go->GetPos_x() + CurrentMap->mapOffset_x;
			int DistanceFromEnemyY = hero.gettheHeroPositiony() - go->GetPos_y();
			CheckEnemiesInRange(go, DistanceFromEnemyX, DistanceFromEnemyY);

			//Checking enemy attack status
			if(go->attackStatus == true)
			{
				go->attackReactionTime += dt;
				if(go->attackReactionTime >= 0.5)
				{
					hero.health--;
					go->attackReactionTime = 0;
					go->attackStatus = false;
					attackAnimation = true;
				}
			}

			else
			{
				go->attackReactionTime = 0;
			}

			//Attacking animation for enemy
			if(attackAnimation == true)
			{
				attackAnimationTimer += dt;
				if(attackAnimationTimer >= 0.7)
				{
					attackAnimationTimer = 0;
					attackAnimation = false;
				}
			}

			//Movement Sprite Animation
			if(go->direction == Vector3(0, -1, 0))
			{
				//Sprite Animation
				go->enemyTileID += 0.1;
				if(go->enemyTileID < 0)
				{
					go->enemyTileID = 0;
				}

				else if(go->enemyTileID > 3)
				{
					go->enemyTileID = 0;
				}
			}

			else if(go->direction == Vector3(-1, 0, 0))
			{
				//Sprite Animation
				go->enemyTileID += 0.1;
				if(go->enemyTileID < 5)
				{
					go->enemyTileID = 5;
				}

				else if(go->enemyTileID > 8)
				{
					go->enemyTileID = 5;
				}
			}

			else if(go->direction == Vector3(1, 0, 0))
			{
				//Sprite Animation
				go->enemyTileID += 0.1;
				if(go->enemyTileID < 10)
				{
					go->enemyTileID = 10;
				}

				else if(go->enemyTileID > 13)
				{
					go->enemyTileID = 10;
				}
			}

			else if(go->direction == Vector3(0, 1, 0))
			{
				//Sprite Animation
				go->enemyTileID += 0.1;
				if(go->enemyTileID < 15)
				{
					go->enemyTileID = 15;
				}

				else if(go->enemyTileID > 18)
				{
					go->enemyTileID = 15;
				}
			}

			cout << "Distance: " << DistanceFromEnemyX << "," << DistanceFromEnemyY << endl;
		}

		//cout << go->direction << endl;
		//cout << "Reaction Time:" << go->attackReactionTime << endl;
	}

	// =================================== UPDATE THE GOODIES ===================================

	for(std::vector<CGoodies *>::iterator it = GoodiesList.begin(); it != GoodiesList.end(); ++it)
	{
		CGoodies *go = (CGoodies *)*it;
		if(go->active)	
		{
			if(hero.GetAttackStatus())
			{
				if(go->GoodiesType == CGoodies::Goodies_Type::BARREL)
				{
					int tileTopLeft_x = (int) ((CurrentMap->mapOffset_x + hero.gettheHeroPositionx()) / CurrentMap->GetTileSize());
					int tileTopLeft_y = CurrentMap->GetNumOfTiles_Height() - (int)ceil( (float)(hero.gettheHeroPositiony() + CurrentMap->GetTileSize()) / CurrentMap->GetTileSize());

					if(CurrentMap->theScreenMap[tileTopLeft_y][tileTopLeft_x - 1] == CMap::BARREL || CurrentMap->theScreenMap[tileTopLeft_y][tileTopLeft_x + 1] == CMap::BARREL || CurrentMap->theScreenMap[tileTopLeft_y - 1][tileTopLeft_x] == CMap::BARREL || CurrentMap->theScreenMap[tileTopLeft_y + 1][tileTopLeft_x] == CMap::BARREL)
					{	
						for(std::vector<CGoodies *>::iterator it = BarrelList.begin(); it != BarrelList.end(); ++it)
						{
							CGoodies *go = (CGoodies *)*it;
							if(go->active)
							{
								int tile_x = go->GetPos_x() / CurrentMap->GetTileSize();
								int tile_y = CurrentMap->GetNumOfTiles_Height() - (go->GetPos_y() + CurrentMap->GetTileSize()) / CurrentMap->GetTileSize();

								if(tile_y == tileTopLeft_y || tile_y == tileTopLeft_y - 1 || tile_y == tileTopLeft_y + 1)
								{
									if(tile_x == tileTopLeft_x || tile_x == tileTopLeft_x - 1 || tile_x == tileTopLeft_x + 1)
									{
										go->active = false;
									}
								}
							}
						}
					}
				}
			}

			else
			{	
				if(go->CalculateDistance(hero.gettheHeroPositionx() + CurrentMap->mapOffset_x, hero.gettheHeroPositiony()) == true)
				{
					if(go->GoodiesType != CGoodies::Goodies_Type::DOOR)
					{
						if(go->GoodiesType == CGoodies::Goodies_Type::KEY)
						{
							hero.SetKeyAcquired(true);
							go->active = false;
							keyCount++;
						}
						
						else if(go->GoodiesType == CGoodies::Goodies_Type::JEWEL)
						{
							go->active = false;
							diamondCount++;
							PointSystem += 10;
						}
					}
				}
			}
			if(go->GoodiesType == CGoodies::Goodies_Type::CHEST)
			{
				if(hero.GetDaggerAcquired() == true)
				{
								go->active = false;
				}
			}
		}
	}

	// =================================== BOSS LEVEL UPDATES ===================================
	
	//map traversing aka character can move from 1 map to another 
	int checkPosition_X = (int)((CurrentMap->mapOffset_x + hero.gettheHeroPositionx()) / CurrentMap->GetTileSize());
	int checkPosition_Y = CurrentMap->GetNumOfTiles_Height() - (int)((hero.gettheHeroPositiony() + CurrentMap->GetTileSize()) / CurrentMap->GetTileSize());

	BossPointer->Set_BossDestination(BossPointer->Get_BossX(), BossPointer->Get_BossY());
	bossCounter += 0.01f;

	if(bossCounter < 2.0f)
	{
		BossTileID += 0.1f;
		if(BossTileID > 2)
		{
			BossTileID = 0;
			IsTurn = false;
		}
	}

	else if(bossCounter > 2.0f && bossCounter < 4.0f)
	{
		BossTileID += 0.1f;
		if(BossTileID > 5)
		{
			BossTileID = 3;
			IsTurn = true;

			for(int i = 0; i < CurrentMap->GetNumOfTiles_Height(); i++)
			{
				for(int k = 0; k < CurrentMap->GetNumOfTiles_Width() + 1; k++)
				{
					if(CurrentMap->theScreenMap[CurrentMap->GetNumOfTiles_Height() - (hero.gettheHeroPositiony() / 32)][hero.gettheHeroPositionx() / 32] == 0)
					{
						EnemiesRendered = true;
					}
				}
			}
		}
	}

	else if(bossCounter > 4.0f)
	{
		bossCounter = 0;
	}

	if(hero.GetdoorOpened() == true)
	{
		derenderDoor = true;
	}

	if(stage == 7 && (CurrentMap->theScreenMap[checkPosition_Y][checkPosition_X + 1] == CMap::BOSS || CurrentMap->theScreenMap[checkPosition_Y + 1][checkPosition_X] == CMap::BOSS || CurrentMap->theScreenMap[checkPosition_Y - 1][checkPosition_X] == CMap::BOSS))
	{
		if(Application::IsKeyPressed(VK_SPACE))
		{
			if(GetKey == false)
			{
				keyCount++;
			}

			if(keyCount == 1)
			{
				hero.SetKeyAcquired(true);
				GetKey = true;
			}

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

	for (std::vector<CEnemy *>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		CEnemy *go = (CEnemy *)*it;
		if (EnemiesRendered == true)
		{
			go->active = true;
		}
		else
		{
			go->active = false;
		}
	}

	BossPointer->Set_SpawnGuards(IsTurn);


	// ============================= MOUSE SECTION ====================================
	//Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;

		if (CustomMap->theScreenMap[CustomMap->GetNumOfTiles_Height() - (x / 32)][y / 32] == 1)
		{
			std::cout << "This is the correct tile" << std::endl;
		}
	}

	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
	}

	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
	}

	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
	}


	// =================================== Lose Screen UPDATES ===================================
	if (hero.health <= 0)
	{
		lose = true;

		if (lose == true)
		{
			LoseTimer += 0.01f;
		}
	}

	if (LoseTimer >= 2.0f)
	{
		LoseTimer = 2.02f;
		lose = false;
		hero.health = 3;
	}

	cout << LoseTimer << endl;


	// =================================== MAIN UPDATES ===================================

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
			if(stage == 1)
			{
				hero.SetKeyAcquired(false);	
				hero.SetdoorOpened(false);
				stage = 2;
				hero.settheHeroPositionx(32);
				hero.heroCurrTile.x = 1;
				enemyList.erase(enemyList.begin(), enemyList.end());
				GoodiesList.erase(GoodiesList.begin(), GoodiesList.end());
				map.InitScrollingMap(enemyList, GoodiesList, BarrelList);
				CurrentMap = map.m_cScrollingMap;
			}

			else if(stage == 2)
			{
				stage = 3;
				hero.settheHeroPositionx(1024 - 32);
				enemyList.erase(enemyList.begin(), enemyList.end());
				GoodiesList.erase(GoodiesList.begin(), GoodiesList.end());
			}
		}
	}

	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	hero.HeroUpdate(CurrentMap, BarrelList, enemyList, CHAR_HEROKEY, stage);
	CHAR_HEROKEY = NULL;
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

void SceneText::CheckEnemiesInRange(CEnemy *go, int DistanceFromEnemyX, int DistanceFromEnemyY)
{
	//Check hero's attack status
	if(hero.GetAttackStatus() == true && stabOnce == false)
	{
		//Check enemies in x-order
		if(DistanceFromEnemyY >= 0 && DistanceFromEnemyY <= 1)
		{
			if(DistanceFromEnemyX >= 0 && DistanceFromEnemyX <= 32)
			{
				//Hero facing left
				if(hero.heroTileID >= 4 && hero.heroTileID <= 6)
				{
					//Backstab enemy
					if(go->direction == Vector3(-1, 0, 0))
					{
						go->health -= 2;
					}

					//Attack enemy from front
					else if(go->direction == Vector3(1, 0, 0))
					{
						go->health--;
					}

					stabOnce = true;
				}
			}

			else if(DistanceFromEnemyX <= 0 && DistanceFromEnemyX >= -32)
			{
				//Hero facing right
				if(hero.heroTileID >= 8 && hero.heroTileID <= 10)
				{
					//Backstab enemy
					if(go->direction == Vector3(1, 0, 0))
					{
						go->health -= 2;
					}

					//Attack enemy from front
					else if(go->direction == Vector3(-1, 0, 0))
					{
						go->health--;
					}

					stabOnce = true;
				}
			}
		}

		//Check enemies in Y-order
		else if(DistanceFromEnemyX >= 0 && DistanceFromEnemyX <= 1)
		{
			if(DistanceFromEnemyY >= 0 && DistanceFromEnemyY <= 32)
			{
				//Hero is facing down
				if(hero.heroTileID >= 0 && hero.heroTileID <= 2)
				{
					//Backstab enemy
					if(go->direction == Vector3(0, -1, 0))
					{
						go->health -= 2;
					}

					//Attack enemy from front
					else if(go->direction == Vector3(0, 1, 0))
					{
						go->health--;
					}

					stabOnce = true;
				}
			}

			else if(DistanceFromEnemyY <= 0 && DistanceFromEnemyY >= -32)
			{
				//Hero is facing up
				if(hero.heroTileID >= 12 && hero.heroTileID <= 14)
				{
					//Backstab enemy
					if(go->direction == Vector3(0, 1, 0))
					{
						go->health -= 2;
					}

					//Attack enemy from front
					else if(go->direction == Vector3(0, -1, 0))
					{
						go->health--;
					}

					stabOnce = true;
				}
			}
		}

		//Assassination from haystack
		else
		{
			if(hero.hiding == true)
			{
				if(hero.GetAttackStatus() == true && stabOnce == false)
				{
					if((DistanceFromEnemyX >= -4 && DistanceFromEnemyX <= 4) && (DistanceFromEnemyY >= -50 && DistanceFromEnemyY <= 50) || (DistanceFromEnemyY >= -4 && DistanceFromEnemyY <= 4) && (DistanceFromEnemyX >= -50 && DistanceFromEnemyX <= 50))
					{
						go->health -= 2;
						stabOnce = true;
					}		
				}
			}
		}
	}

	//Reset for hero to hit again
	else if(hero.GetAttackStatus() == false && stabOnce == true)
	{
		stabOnce = false;
	}

	//Check enemies' health
	if(go->health <= 0)
	{
		go->active = false;
		go->health = 0;
		go->attackReactionTime = 0;
	}

	//Checking if enemy can attack hero
	if((DistanceFromEnemyY >= 0 && DistanceFromEnemyY <= 1) && (DistanceFromEnemyX >= -32 && DistanceFromEnemyX <= 32) || (DistanceFromEnemyX >= 0 && DistanceFromEnemyX <= 1) && (DistanceFromEnemyY >= -32 && DistanceFromEnemyY <= 32))
	{
		go->attackStatus = true;
	}

	else
	{
		go->attackStatus = false;
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
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2.3, 71, 0.5);

	if(Application::IsKeyPressed(VK_INSERT))
	{
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
		ss3 << "TileOffset_x:" << CurrentMap->tileOffset_x;
		RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(1, 1, 0), 2.3, 2, 49);
	}
}

void SceneText::RenderHero()
{
	//For dislaying Hero's Health
	if(hero.health >= 1)
	{
		Render2DMesh(meshList[GEO_HUD_HEART], false, 20, hero.gettheHeroPositionx() - 15, hero.gettheHeroPositiony() + 33);

		if(hero.health >= 2)
		{
			Render2DMesh(meshList[GEO_HUD_HEART], false, 20, hero.gettheHeroPositionx() + 6, hero.gettheHeroPositiony() + 33);	
			
			if(hero.health == 3)
			{
				Render2DMesh(meshList[GEO_HUD_HEART], false, 20, hero.gettheHeroPositionx() + 27, hero.gettheHeroPositiony() + 33);
			}
		}
	}

	//Attacking
	if(hero.GetAttackStatus() == true)
	{
		if(hero.heroTileID >= 0 && hero.heroTileID <= 2)
		{
			RenderSprites(meshList[GEO_TILEHEROSHEET], 3, 32, hero.gettheHeroPositionx(), hero.gettheHeroPositiony());
		}

		else if(hero.heroTileID >= 4 && hero.heroTileID <= 6)
		{
			RenderSprites(meshList[GEO_TILEHEROSHEET], 7, 32, hero.gettheHeroPositionx(), hero.gettheHeroPositiony());
		}

		else if(hero.heroTileID >= 8 && hero.heroTileID <= 10)
		{
			RenderSprites(meshList[GEO_TILEHEROSHEET], 11, 32, hero.gettheHeroPositionx(), hero.gettheHeroPositiony());
		}

		else
		{
			RenderSprites(meshList[GEO_TILEHEROSHEET], 15, 32, hero.gettheHeroPositionx(), hero.gettheHeroPositiony());
		}
	}

	//Walking
	else
	{
		RenderSprites(meshList[GEO_TILEHEROSHEET], hero.heroTileID, 32, hero.gettheHeroPositionx(), hero.gettheHeroPositiony());
	}
}

void SceneText::RenderEnemies()
{
	//For displaying Enemy's Health
	for(int i = 0; i < enemyList.size(); ++i)
	{
		if(enemyList[i]->health >= 1)
		{
			Render2DMesh(meshList[GEO_HUD_HEART], false, 20, (enemyList[i]->GetPos_x() - 5) - CurrentMap->mapOffset_x, enemyList[i]->GetPos_y() - 20);

			if(enemyList[i]->health == 2)
			{
				Render2DMesh(meshList[GEO_HUD_HEART], false, 20, (enemyList[i]->GetPos_x() + 15) - CurrentMap->mapOffset_x, enemyList[i]->GetPos_y() - 20);
			}
		}
	}

	for(vector<CEnemy *>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		CEnemy *go = (CEnemy *)*it;
		int theEnemy_x = go->GetPos_x() - CurrentMap->mapOffset_x;
		int theEnemy_y = go->GetPos_y();
		Vector3 theEnemyPos (theEnemy_x,theEnemy_y,0);

		if(go->active)	
		{	
			//Detection radius
			for(vector<Vector3>::iterator it2 = go->detectionGrid.begin(); it2 != go->detectionGrid.end(); ++it2)
			{
				if(go->theStrategy->CurrentState == CStrategy::PATROL)
				{
					Vector3 go2 = (Vector3)*it2;
					Render2DMesh(meshList[GEO_TILEDETECTIONRADIUS], false, 1.0f, go2.x - CurrentMap->mapOffset_x, go2.y);
				}

				else if(go->theStrategy->CurrentState == CStrategy::ATTACK)
				{
					Render2DMesh(meshList[GEO_TILEEXCLAMATIONMARK], false, 23, (go->GetPos_x() + 5) - CurrentMap->mapOffset_x, go->GetPos_y() + 35);
				}
			}

			//Attacking
			if(attackAnimation == true)
			{
				if(go->direction == Vector3(0, -1, 0))
				{
					RenderSprites(meshList[GEO_TILEENEMYSHEET], 3, 32, theEnemy_x, theEnemy_y);
				}

				else if(go->direction == Vector3(-1, 0, 0))
				{
					RenderSprites(meshList[GEO_TILEENEMYSHEET], 8, 32, theEnemy_x, theEnemy_y);
				}

				else if(go->direction == Vector3(1, 0, 0))
				{
					RenderSprites(meshList[GEO_TILEENEMYSHEET], 13, 32, theEnemy_x, theEnemy_y);
				}

				else
				{
					RenderSprites(meshList[GEO_TILEENEMYSHEET], 18, 32, theEnemy_x, theEnemy_y);
				}
			}

			//Walking
			else
			{		
				RenderSprites(meshList[GEO_TILEENEMYSHEET], go->enemyTileID, 32, theEnemy_x, theEnemy_y);
			}
		}

		// Idling
		else if (go->active == false && stage == 7)
		{
			RenderSprites(meshList[GEO_TILEENEMYSHEET], 5, 32, theEnemy_x, theEnemy_y);
		}

		//Dead
		else
		{
			RenderSprites(meshList[GEO_TILEENEMYSHEET], 19, 32, theEnemy_x, theEnemy_y);
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

			if(stage == 1)
			{
				if(CurrentMap->theScreenMap[i][m] >= 20 && CurrentMap->theScreenMap[i][m] <= 49)
				{
					int tempMesh = CurrentMap->theScreenMap[i][m] - 20;
					Render2DMesh(meshList[GEO_20 + tempMesh], false, 1.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x, 768 - i * CurrentMap->GetTileSize());
				}

				else if(CurrentMap->theScreenMap[i][m] == CMap::DOOR)
				{
					if(hero.GetdoorOpened() == false)
					{
						Render2DMesh(meshList[GEO_TILEDOOR], false, 1.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x, 768 - i * CurrentMap->GetTileSize());
					}
					
					else
					{
						Render2DMesh(meshList[GEO_TILEBACKGROUND], false, 1.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x, 768 - i * CurrentMap->GetTileSize());
					}
				}

				else
				{
					Render2DMesh(meshList[GEO_TILEBACKGROUND], false, 1.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x, 768 - i * CurrentMap->GetTileSize());
				}
			}

			else if(stage == 2)
			{
				if (CurrentMap->theScreenMap[i][m] >= 20 && CurrentMap->theScreenMap[i][m] <= 49)
				{
					int tempMesh = CurrentMap->theScreenMap[i][m] - 20;
					Render2DMesh(meshList[GEO_20 + tempMesh], false, 1.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x, 768 - i * CurrentMap->GetTileSize());
				}

				else if(CurrentMap->theScreenMap[i][m] == CMap::DOOR)
				{
					Render2DMesh(meshList[GEO_TILEDOOR], false, 1.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x, 768 - i * CurrentMap->GetTileSize());
				}

				else
				{
					Render2DMesh(meshList[GEO_TILEBACKGROUND], false, 1.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x, 768 - i * CurrentMap->GetTileSize());
				}
			}

			else if (stage == 7)
			{
				if (CurrentMap->theScreenMap[i][m] >= 20 && CurrentMap->theScreenMap[i][m] <= 49)
				{
					int tempMesh = CurrentMap->theScreenMap[i][m] - 20;
					Render2DMesh(meshList[GEO_20 + tempMesh], false, 1.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x, 768 - i * CurrentMap->GetTileSize());
				}

				else
				{
					Render2DMesh(meshList[GEO_TILEBACKGROUND], false, 1.0f, k * CurrentMap->GetTileSize() - CurrentMap->mapFineOffset_x, 768 - i * CurrentMap->GetTileSize());
				}

				RenderSprites(meshList[GEO_TILEBOSS_FRAME0], BossTileID, 64, 55, 400);
			}
		}
	}
}

void SceneText::RenderGameOver()
{
	RenderQuadOnScreen(meshList[GEO_LOSE], 82, 62, 40, 30, false);
}

void SceneText::RenderGoodies()
{
	//Render the goodies
	for (vector<CGoodies *>::iterator it = GoodiesList.begin(); it != GoodiesList.end(); ++it)
	{
		CGoodies *go = (CGoodies *)*it;
		if (go->active)
		{
			int theGoodies_x = go->GetPos_x() - map.mapOffset_x;
			int theGoodies_y = go->GetPos_y();

			if (go->GoodiesType == CGoodies::Goodies_Type::JEWEL)
			{
				if (go->active == true)
				{
					Render2DMesh(meshList[GEO_DIAMOND], false, 1.0f, theGoodies_x - CurrentMap->mapOffset_x, theGoodies_y);
				}
			}
			else if (go->GoodiesType == CGoodies::Goodies_Type::KEY)
			{
				if (go->active == true)
				{
					Render2DMesh(meshList[GEO_KEY], false, 1.0f, theGoodies_x - CurrentMap->mapOffset_x, theGoodies_y);
				}
			}
			else if (go->GoodiesType == CGoodies::Goodies_Type::CHEST)
			{
				if (go->active == true)
				{
					Render2DMesh(meshList[GEO_CHEST], false, 1.0f, theGoodies_x - CurrentMap->mapOffset_x, theGoodies_y);
				}
				else
				{
					Render2DMesh(meshList[GEO_CHEST_OPENED], false, 1.0f, theGoodies_x - CurrentMap->mapOffset_x, theGoodies_y);
				}
			}
			else if (go->GoodiesType == CGoodies::Goodies_Type::BARREL)
			{
				if (go->active)
				{
					Render2DMesh(meshList[GEO_BARREL], false, 1.0f, theGoodies_x - CurrentMap->mapOffset_x, theGoodies_y);
				}
				else
				{
					Render2DMesh(meshList[GEO_BARREL_BROKEN], false, 1.0f, theGoodies_x - CurrentMap->mapOffset_x, theGoodies_y);
				}

			}

			else if (go->GoodiesType == CGoodies::Goodies_Type::HAY)
			{
				Render2DMesh(meshList[GEO_HAY], false, 1.0f, theGoodies_x - CurrentMap->mapOffset_x, theGoodies_y);
			}

			else if (go->GoodiesType == CGoodies::Goodies_Type::HOLE)
			{
				Render2DMesh(meshList[GEO_HOLE], false, 1.0f, theGoodies_x - CurrentMap->mapOffset_x, theGoodies_y);
			}
		}
	}
}

void SceneText::RenderHUD()
{
	//For indicating number of diamonds collected
	RenderQuadOnScreen(meshList[GEO_HUD_DIAMOND], 3.4, 3, 1, 56.5, false);

	std::ostringstream ss1;
	ss1.precision(5);
	ss1 << "x " << diamondCount;
	RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 0, 1), 2.3, 5, 57);

	//For indicating number of keys collected
	RenderQuadOnScreen(meshList[GEO_HUD_KEY], 3.4, 3, 12, 56.5, false);

	std::ostringstream ss2;
	ss2.precision(5);
	ss2 << "x " << keyCount;
	RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0.5, 0.5, 0.5), 2.3, 16, 57);

	//For Point System
	std::ostringstream ss3;
	ss3.precision(5);
	ss3 << "Points: " << PointSystem;
	RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(1, 0, 0), 2.3, 65, 57);
}

void SceneText::RenderMenu(int &InteractHighLight, int max, int min)
{
	if(Application::IsKeyPressed(VK_DOWN) && delay == 0 && InteractHighLight < max)
	{
		InteractHighLight += 1;
		delay = 15;
	}

	if(Application::IsKeyPressed(VK_UP) && delay == 0 && InteractHighLight > min)
	{
		InteractHighLight -= 1;
		delay = 15;
	}

	if(delay > 0)
	{
		--delay;
	}

	if(InteractHighLight == 0 && Application::IsKeyPressed(VK_RETURN))
	{
		menu = false;
	}

	if(InteractHighLight == 1 && Application::IsKeyPressed(VK_RETURN))
	{
		menu = false;
	}

	//Menu
	int a = 0;
	if(menu == true)
	{
		for(int text = 0; text < 2; text++)
		{
			float TextSize = 5;
			int y = 60 / TextSize / 2 - 5 - (text * TextSize);

			if(InteractHighLight == text)
			{
				a = 1;
			}

			RenderTextOnScreen(meshList[GEO_TEXT], Text[text], Color(1, a, a), TextSize, 40, y + 30);

			if(InteractHighLight == text)
			{
				a = 0;
			}
		}
	}
}

void SceneText::Render()
{
	/*if(menu == true)
	{
		RenderQuadOnScreen(meshList[GEO_MENU], 82, 62, 40, 30, false);
		RenderMenu(InteractHighLight, 1, 0);
	}*/

	RenderInit();
	RenderTileMap();
	RenderEnemies();
	RenderHero();
	RenderGoodies();
	RenderText();
	RenderHUD();

	if (lose == true)
	{
		RenderGameOver();
	}
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