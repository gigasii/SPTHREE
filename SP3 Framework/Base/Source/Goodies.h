#pragma once 
 
#include "Mesh.h" 
 
class CGoodies 
{ 

protected: 
	// Position x of this goodies  	
	int pos_x; 
	// Position y of this goodies  	
	int pos_y;

public: 
 	CGoodies();  	
	~CGoodies(); 

	enum Goodies_Type
	{
		JEWEL = 0,
		KEY,
		CHEST,
		DOOR,
		GOODIES_TOTAL
	};

 	// Stores the texture for this goodies 
 	Mesh* theMesh; 
 
 	// Get the pos_x of this goodies 
 	int GetPos_x(void); 
 	
	// Get the pos_y of this goodies 
 	int GetPos_y(void); 
 	
	// Set the pos_x and pos_y of this goodies  	
	void SetPos(const int pos_x, const int pos_y); 
 
 	// Get the Mesh 
 	Mesh* GetMesh(void);  	
	
	// Set the Mesh 
 	void SetMesh(Mesh* theMesh);  	
	
	// Set the Texture ID 
 	void SetTextureID(const int TextureID); 

	//check whether is active
	bool active;
	//calculate distance from player to see if can be pick up
	bool CalculateDistance(int pos_x, int pos_y);

	Goodies_Type GoodiesType;
}; 


