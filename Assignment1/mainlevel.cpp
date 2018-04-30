#include "mainlevel.h"
#include "modelloader.h"

void MainLevel::Initialize()
{
	//Create Camera(s) for the level
	m_pCamera = new Camera();

	//Creates all objects in the level.
	//IF WANT NEW LEVEL:
	//		- Create a new class override Level's Initialize for different levels

	// Cat Box
	Entity* cat = new Entity(ModelLoader::Instance().GetModel("Jeffrey"), m_pCamera);
	cat->Initialize(glm::vec3(0, -5, 15), glm::vec3(1),
		glm::vec3(0, 1, 0), 0, glm::vec3(0.7f, 0.7f, 0.5f));
	cat->EnableStencil(glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX));
	this->AddEntity(cat);
	
	// Star
	glPointSize(5);
	m_pStar = new GeometryModel(m_pCamera, glm::vec3(0, 0, 5));

	// Terrain
	m_pTerrain = new Terrain();
	m_pTerrain->Initialize(m_pCamera, "Resources/Images/heightmap1.jpg", glm::vec3(0, -15, 15));
}

void MainLevel::Update()
{
	Level::Update();

	const int camSpeed = 10;

	if (Input::Instance().GetKey('w'))
	{
		glm::vec3 newPos = m_pCamera->GetPosition() + glm::vec3(0, 0, Time::Instance().DeltaTime() * camSpeed);
		m_pCamera->UpdateCameraPosition(newPos);
	}
	else if (Input::Instance().GetKey('s'))
	{
		glm::vec3 newPos = m_pCamera->GetPosition() - glm::vec3(0, 0, Time::Instance().DeltaTime() * camSpeed);
		m_pCamera->UpdateCameraPosition(newPos);
	}

	if (Input::Instance().GetKey('a'))
	{
		glm::vec3 newPos = m_pCamera->GetPosition() + glm::vec3(Time::Instance().DeltaTime() * camSpeed, 0, 0);
		m_pCamera->UpdateCameraPosition(newPos);
	}
	else if (Input::Instance().GetKey('d'))
	{
		glm::vec3 newPos = m_pCamera->GetPosition() - glm::vec3(Time::Instance().DeltaTime() * camSpeed, 0, 0);
		m_pCamera->UpdateCameraPosition(newPos);
	}
	//Reset scene if player presses 'R'
	if (Input::Instance().GetKeyDown('r'))
	{
		CleanUp();
		Initialize();
	}

	// Move box so it sits on terrain
	glm::vec3 position = m_vecEntity[0]->Position();
	float newHeight = m_pTerrain->GetHeightOnTerrain(position);
	m_vecEntity[0]->SetPosition(glm::vec3(m_pCamera->GetPosition().x, newHeight, m_pCamera->GetPosition().z + 35));
}