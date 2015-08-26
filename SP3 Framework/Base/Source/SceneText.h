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
		GEO_SPHERE2,
		GEO_AIM,
		GEO_CUBE,
		GEO_QUAD,
		GEO_TEXT,

		//Tilesheet
		GEO_TILEBACKGROUND,
		GEO_TILEDOOR,
		GEO_TILEDETECTIONRADIUS,

		//Hero sprite Animation
		GEO_TILEHEROSHEET,

		//Customisation Menu
		GEO_HERO_RED,
		GEO_HERO_BLUE,
		GEO_BLUE_SPRITE,
		GEO_CUSTOM_MENU,

		//Enemy sprite Animation
		GEO_TILEENEMYSHEET,
		GEO_TILEENEMYSHEET2,
		GEO_EXCLAMATIONMARK,

		//Boss Sprite Animation
		GEO_TILEBOSS_FRAME0,

		//Goodies
		GEO_DIAMOND,
		GEO_KEY,
		GEO_CHEST,
		GEO_CHEST_OPENED,
		GEO_BARREL,
		GEO_BARREL_BROKEN,
		GEO_HAY,
		GEO_HOLE,

		//HUD icons
		GEO_HUD_HEART,
		GEO_HUD_KEY,
		GEO_HUD_DIAMOND,
		GEO_DETECTIONEYE,
		GEO_DETECTIONEYE2,

		//Game Screens
		GEO_MENU,
		GEO_LOSE,

		//Sahara desert
		GEO_TILESHEET_DESERT,
	
		GEO_DIM,

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
	
	//Update Camera status
	virtual void UpdateCameraStatus(const unsigned char key, const bool status = true);
	
	//Update Attack status
	virtual void UpdateAttackStatus(const unsigned char key);

	//Check whether hero is in range to attack enemies
	void CheckEnemiesInRange(CEnemy *go, Hero hero, int DistanceFromEnemyX, int DistanceFromEnemyY);

	//General functions to run the program
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size = 1.0f, float x = 0.0f, float y = 0.0f, bool rotate = false);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void Render2DMesh(Mesh *mesh, const bool enableLight, const float size = 1.0f, const float x = 0.0f, const float y = 0.0f, const bool rotate = false);
	void RenderQuadOnScreen(Mesh* mesh, float sizeX, float sizeY, float x, float y, bool enableLight);
	void RenderSprites(Mesh* mesh, int id, const float size, const float x, const float y);
	void RenderTilesMap(Mesh* mesh, int ID, const float size, const float x, const float y);
	void RenderInit();
	void RenderText();
	void RenderHero();
	void RenderEnemies();
	void RenderHUD();
	void RenderGameOver();
	void RenderBoss();
	void RenderCustomMenu();
	void RenderMinimap();
	void RenderMenu(int &InteractHighLight, int max, int min);

	//General tilemap renderer
	void RenderTileMap();

	//Render Goodies
	void RenderGoodies();

	static bool bReset;

	bool checkCollision(GameObject* go, GameObject* go2, double dt);
	void collisionResponse(GameObject* go, GameObject* go2);

	void RenderGO(GameObject *go);

	GameObject* FetchGO();

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
	
	//current map/map handler
	CMap *CurrentMap;
	
	//Boss handler
	CBoss *BossPointer;

	//Vector list of enemies
	std::vector<CEnemy *> enemyList;

	//Vector of GoodieList
	std::vector<CGoodies *> GoodiesList;
	std::vector<CGoodies *> BarrelList;
	std::vector<CGoodies *> HoleList;
	std::vector<GameObject *> m_goList;

	//General variables
	float fps;
	float rotateAngle;
	float bossCounter;

	bool EnemiesRendered;
	bool bLightEnabled;
	bool IsTurn;
	bool GetKey;
	bool derenderDoor;
	float BossTileID;

	//Project variables
	int stage;
	float attackSpeed;
	bool stabOnce;
	bool RenderDim;

	//HUD Variable
	int diamondCount;
	int keyCount;
	int PointSystem;

	//Main Menu Variables
	int delay;
	int InteractHighLight;
	bool menu;
	string Text[2];

	//Custom Menu Variables
	bool CustomMenuRendered, CustomMenuSelected;
	bool Blue_Selected, Red_Selected, Temp_Red_Selected;
	bool CheckCustomMenu;
	int Custom_HeroSize_Red, Custom_HeroSize_Blue;
	int CloseOpenCustomMenu;
	float CustomMenuDelay;
	
	//GameOver Variables
	bool lose;
	float LoseTimer;

	//Storing Value
	int a, b;
	GameObject *m_ghost;

	// For Minimap
	CMinimap* m_cMinimap;

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;
	Vector3 prevPos;
	bool onHero;
	bool lockMovement;

protected:
	float m_worldWidth;
	float m_worldHeight;
};

#endif