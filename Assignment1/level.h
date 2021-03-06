#pragma once

#include "include.h"
#include "camera.h"
#include "entity.h"
#include "quad.h"
#include "cube.h"
#include "terrain.h"
#include "geometry_model.h"

class Level
{
//== Methods
public:
	Level();
	~Level();

	//Fill level with objects. Initialize all objects
	virtual void Initialize() = 0;
	//Destroy all objects
	virtual void CleanUp();
	//Update all objects. Apply level logic
	virtual void Update();
	//Render all objects
	virtual void Render();

	void AddEntity(Entity* _newEntity);
	void AddQuad(Quad* _newQuad);
	void AddCube(Cube* _newCube);

protected:
	Level(const Level&) = delete;
	void operator=(Level const&) = delete;

//== Members
protected:
	Camera* m_pCamera;
	std::vector<Entity*> m_vecEntity;
	std::vector<Quad*> m_vecQuads;
	std::vector<Cube*> m_vecCubes;
	Terrain* m_pTerrain;
	GeometryModel* m_pStar;
};