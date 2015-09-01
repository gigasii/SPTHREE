#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Hero.h"
#include "Enemy.h"
#include "Boss.h"
#include "GameObject.h"
#include "Minimap.h"
#include"HighscoreManager.h"

class SceneText : public Scene
{
	static const int MAX_SPEED = 800;

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_SPHERE,
		GEO_CUBE,
		GEO_QUAD,
		GEO_TEXT,

		//Tilesheet
		GEO_TILEBACKGROUND,
		GEO_TILEDOOR,
		GEO_TILEDETECTIONRADIUS,

		//Hero sprite Animation
		GEO_TILEHEROSHEET,
		GEO_TILEHEROSHEET2,
		GEO_TILEHEROSHEET3,
		GEO_TRANSFORMATIONSHEET,

		//Customisation Menu
		GEO_HERO_RED,
		GEO_HERO_BLUE,
		GEO_CUSTOM_MENU,

		//Enemy sprite Animation
		GEO_TILEENEMYSHEET,
		GEO_TILEENEMYSHEET2,
		GEO_TILEENEMYSHEET3,
		GEO_EXCLAMATIONMARK,
		GEO_STUNSHEET,

		//For Introduction Screen and Name
		GEO_INTRO_SCREEN,
		GEO_NAME_SCREEN,

		//Boss Sprite Animation
		GEO_TILEBOSS_FRAME0,
		GEO_BOSS2,

		//Goodies
		GEO_DIAMOND,
		GEO_KEY,
		GEO_CHEST,
		GEO_CHEST_OPENED,
		GEO_BARREL,
		GEO_BARREL_BROKEN,
		GEO_HAY,
		GEO_HOLE,
		
		//Shop items
		GEO_HPPOT,
		GEO_MAXHP,
		GEO_SCROLL,

		//HUD icons
		GEO_HUD_HEART,
		GEO_HUD_KEY,
		GEO_HUD_DIAMOND,
		GEO_HUD_SHURIKEN,
		GEO_DETECTIONEYE,
		GEO_DETECTIONEYE2,
		GEO_DIM,
		GEO_STAMINAMETER,
		
		//Weapon
		GEO_AIM,
		GEO_SHURIKEN,
		GEO_DAGGER,

		//Game Screens
		GEO_MENU,
		GEO_LOSE,
		GEO_WIN,
		GEO_STAGECLEAR,

		//Sahara desert
		GEO_TILESHEET_DESERT,

		//Text
		GEO_SELECTOR,
		GEO_SELECTOR2,
		GEO_TILESHEET_SELECTOR,
		GEO_TEXT2,

		NUM_GEOMETRY,
	};

public:
	SceneText();
	~SceneText();

	//Main functions to run program
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//Minimap Init Functions
	void InitMiniMap_Level1();
	void InitMiniMap_Level2();
	void InitMiniMap_Level3();
	void InitMiniMap_Level4();
	void InitMiniMap_Level5();
	void InitMiniMap_Level6();
	void InitMiniMap_Level7();
	void InitMiniMap_Level8();

	//General functions to run the program
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size = 1.0f, float x = 0.0f, float y = 0.0f, bool rotate = false);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void Render2DMesh(Mesh *mesh, const bool enableLight, const float size = 1.0f, const float x = 0.0f, const float y = 0.0f, const bool rotate = false);
	void RenderQuadOnScreen(Mesh* mesh, float sizeX, float sizeY, float x, float y, bool enableLight);
	void RenderSprites(Mesh* mesh, int id, const float size, const float x, const float y);
	void RenderTilesMap(Mesh* mesh, int ID, const float size, const float x, const float y);

	//Updates
	virtual void UpdateCameraStatus(const unsigned char key, const bool status = true);
	virtual void UpdateAttackStatus(const unsigned char key);
	void CheckEnemiesInRange(CEnemy *go, Hero hero, int DistanceFromEnemyX, int DistanceFromEnemyY);
	void UpdateHero(double dt);
	void UpdateEnemies(double dt);
	void UpdateGoodies(double dt);
	void UpdateBossLevel(int checkPosition_X, int checkPosition_Y);
	void UpdateCustomisation(double dt);
	void UpdateGameOver(double dt);
	void UpdateMouse();
	void UpdatePhysics(double dt);
	void UpdateMiniMap(double dt);
	void UpdateBossLevelScrolling();
	void UpdateLevels(int checkPosition_X, int checkPosition_Y, double dt);
	void UpdateHighscore();
	void UpdateName(double dt);
	
	//Game project functions
	void RenderInit();
	void RenderHero();
	void RenderEnemies();
	void RenderTileMap();
	void RenderGoodies();
	void RenderBoss();
	void RenderHUD();
	void RenderMinimap();
	void RenderHighscore();
	void RenderName();
	
	//Render Menus
	void RenderMenu(int &InteractHighLight, int max, int min);
	void RenderGameOver();
	void RenderCustomMenu();
	void RenderWeaponCollectedMenu();
	void RenderStageClear();

	//Physics functions
	bool checkCollision(GameObject* go, GameObject* go2, double dt);
	void collisionResponse(GameObject* go, GameObject* go2);
	void RenderGO(GameObject *go);
	GameObject* FetchGO();

	static bool bReset;

enum CHARACTER_ACTION
{
	CA_NIL = 0,
	CA_ATTACK,
	
	CA_TOTAL,
};

private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	Mesh* meshList[NUM_GEOMETRY];
	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Camera3 camera;
	Light lights[2];
	Hero hero;
	CMap map;
	CBoss boss;
	CMinimap minimap;

	//current map/map handler
	CMap *CurrentMap;
	
	//Current Minimap Handler
	CMinimap *CurrentMiniMap;

	//Boss handler
	CBoss *BossPointer;

	//Vector list of enemies
	std::vector<CEnemy *> enemyList;

	//Vector of GoodieList
	std::vector<CGoodies *> GoodiesList;
	std::vector<CGoodies *> BarrelList;
	std::vector<CGoodies *> HoleList;
	std::vector<GameObject *> m_goList;

	//Project variables
	float fps;
	float rotateAngle;
	bool InShop;
	int stage;
	bool stabOnce;
	bool RenderDim;
	bool chestOpen;
	float floatUp;
	bool weaponCollectedScreen;
	float weaponCollectedTimer;
	bool stageClear;
	float stageClearTimer;
	float floatDown;
	bool gunshot;
	bool pickweaponsound;
	bool smokescreen;
	bool hiding;
	bool stageclearsound;
	bool walking;
	bool stun;

	//Boss variables
	float bossCounter;
	bool EnemiesRendered;
	bool bLightEnabled;
	bool IsTurn;
	bool GetKey;
	bool derenderDoor;
	float BossTileID;
	int CurrentScreenMapX;
	int CurrentScreenMapY;

	//HUD Variable
	int diamondCount;
	int keyCount;
	int PointSystem;

	//Main Menu Variables
	int delay;
	int InteractHighLight;
	bool menu;
	bool name;
	bool instruc;
	string Text[2];

	//Minimap Variables
	bool MiniMapRendered;

	//Custom Menu Variables
	bool CustomMenuRendered, CustomMenuSelected;
	bool Blue_Selected, Red_Selected, Temp_Red_Selected;
	bool CheckCustomMenu;
	int Custom_HeroSize_Red, Custom_HeroSize_Blue;
	int CloseOpenCustomMenu;
	float CustomMenuDelay;
	
	//GameOver Variables
	bool lose;
	bool win;
	float LoseTimer;
	float winTimer;

	//Storing Value
	int a, b;
	GameObject *m_ghost;

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;
	Vector3 prevPos;
	bool onHero;
	bool lockMovement;

	//Highscore
	CHighscoreManager Highscore;
	CHighscore PlayerScore;

	//Name entering
	bool nameMenu;
	Vector3 selectorTile;
	Vector3 selector2Tile;
	double selectorTimer, selectorTimer2;
	bool selectorRender;
	int namePos;
	int ASCIIconvert (Vector3 tile);
	string playerName;

protected:
	float m_worldWidth;
	float m_worldHeight;
};

#endif