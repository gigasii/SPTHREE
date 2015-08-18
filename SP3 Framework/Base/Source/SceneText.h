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

class SceneText : public Scene
{
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
		GEO_QUAD,
		GEO_TEXT,

		//Tilesheet
		GEO_SCREENTILESHEET,
		GEO_TILEBACKGROUND,

		//Hero sprite Animation
		GEO_TILEHEROSHEET,
		GEO_TILEHEROSHEET2,

		//Enemy sprite Animation
		GEO_TILEENEMY_FRAME0,

		//Goodies
		GEO_DIAMOND,
		GEO_KEY,
		GEO_CHEST,
		
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

	//General tilemap renderer
	void RenderTileMap();

	//Render Goodies
	void RenderGoodies();

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
	CMap * CurrentMap;

	//Vector list of enemies
	std::vector<CEnemy *> enemyList;

	//Vector of GoodieList
	std::vector<CGoodies *> GoodiesList;

	//General variables
	float fps;
	float rotateAngle;
	bool  bLightEnabled;

	//Project variables
	int level;
	float attackSpeed;

	//Sound effects

	//Sprites Variable
	int heroTileID;
};

#endif