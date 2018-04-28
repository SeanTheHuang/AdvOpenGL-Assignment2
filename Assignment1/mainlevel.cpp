#include "mainlevel.h"
#include "modelloader.h"

MainLevel* MainLevel::s_instance = nullptr;

void MainLevel::Initialize()
{
	//Create Camera(s) for the level
	m_pCamera = new Camera();

	//Creates all objects in the level.
	//IF WANT NEW LEVEL:
	//		- Create a new class override Level's Initialize for different levels

	//for (size_t i = 0; i < 10; i++)
	//{
	//	float even = i % 2 == 0 ? 1.0f : -1.0f;

	//	Entity* cat = new Entity(ModelLoader::Instance().GetModel("Jeffrey"), m_pCamera);
	//	cat->Initialize(glm::vec3(12 * even, -5, 20 + 12 * floorf((float)i / 2)), glm::vec3(1),
	//		glm::vec3(0, 1, 0), 45 * even, glm::vec3(0.7f, 0.7f, 0.5f));
	//	cat->EnableStencil(glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX));
	//	this->AddEntity(cat);
	//}

	////Create quad
	//Quad* quad = new Quad(m_pCamera, "Resources/Images/weebLove.png");
	//quad->Initialize(glm::vec3(0, 0, 40), glm::vec3(20), glm::vec3(0, 1, 0), 0, glm::vec4(1, 1, 1, 0.25f));
	//this->AddQuad(quad);

	////Create cube
	//Cube* cube = new Cube(m_pCamera);
	//cube->Initialize(glm::vec3(0, 0, 12), glm::vec3(1), glm::normalize(glm::vec3(0.6f, 0.4f, 0.2f)), 0, glm::vec4(0.2f, 0.6f, 0.3f, 1));
	//this->AddCube(cube);

	//m_pTerrain = new Terrain(m_pCamera, "Resources/Images/weebLove.png");
	//m_pTerrain->Initialize(glm::vec3(0, -2, 0));

	//Set up unique level mouse click function
	//s_instance = this;
	//glutMouseFunc(MouseFunc);

	Entity* cat = new Entity(ModelLoader::Instance().GetModel("Jeffrey"), m_pCamera);
	cat->Initialize(glm::vec3(0, -5, 15), glm::vec3(1),
		glm::vec3(0, 1, 0), 0, glm::vec3(0.7f, 0.7f, 0.5f));
	cat->EnableStencil(glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX));
	this->AddEntity(cat);
	

	glPointSize(5);
	m_pStar = new GeometryModel(m_pCamera, glm::vec3(0, 0, 5));
}

void MainLevel::Update()
{
	Level::Update();

	const int camSpeed = 5;

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
}

void MainLevel::MouseFunc(int _button, int _buttonState, int _x, int _y)
{
	//Do good stuff when left mouse click
	if (_button != GLUT_LEFT_BUTTON || _buttonState != GLUT_DOWN)
		return;

	std::vector<Triangle> vecTriangles;
	Ray raycast;
	float shortestDist = 100000000;
	glm::vec3 collisionPoint;

	//Get triangles of the cube
	for (size_t i = 0; i < 192; i += 32)
	{
		vecTriangles.push_back(Triangle(glm::vec3(CubeVertices[i], CubeVertices[i+1], CubeVertices[i+2]),
										glm::vec3(CubeVertices[i+8], CubeVertices[i+9], CubeVertices[i+10]),
										glm::vec3(CubeVertices[i+16], CubeVertices[i+17], CubeVertices[i+18])));

		vecTriangles.push_back(Triangle(glm::vec3(CubeVertices[i+8], CubeVertices[i + 9], CubeVertices[i + 11]),
										glm::vec3(CubeVertices[i + 16], CubeVertices[i + 17], CubeVertices[i + 18]),
										glm::vec3(CubeVertices[i + 24], CubeVertices[i + 25], CubeVertices[i + 26])));
	}

	//Transform each point to world space
	glm::mat4 model = s_instance->m_vecCubes[0]->ModelMatrix();
	for (size_t i = 0; i < vecTriangles.size(); i++)
	{
		vecTriangles[i].p1 = TransformPoint(vecTriangles[i].p1, model);
		vecTriangles[i].p2 = TransformPoint(vecTriangles[i].p2, model);
		vecTriangles[i].p3 = TransformPoint(vecTriangles[i].p3, model);
	}

	//Transform mouse position to ray position
	raycast.origin = s_instance->m_pCamera->GetPosition();
	raycast.dir = GetRayDirection((float)_x, (float)_y);

	//Check every triangle, 
	//record the hit with lowest distance to camera
	for (size_t i = 0; i < vecTriangles.size(); i++)
	{
		
		//Ray points
		glm::vec3 r1 = raycast.origin; //Close plane
		glm::vec3 r2 = raycast.origin + 1000.0f * raycast.dir; //far plane

		////Get normal of triangle face
		glm::vec3 triangleNormal = glm::normalize(glm::cross(
			vecTriangles[i].p2 - vecTriangles[i].p1, vecTriangles[i].p3 - vecTriangles[i].p1));
		triangleNormal = glm::normalize(triangleNormal);

		float dist1 = glm::dot(r1 - vecTriangles[i].p1, triangleNormal);
		float dist2 = glm::dot(r2 - vecTriangles[i].p1, triangleNormal);

		//Line does not cross triangle
		if (dist1 * dist2 >= 0.0f)
			continue;

		//Line and plane are parallel
		if (dist1 == dist2)
			continue;

		glm::vec3 intersection = r1 + (r2 - r1) * (-dist1 / (dist2 - dist1));

		//Check if intersection point lies inside triangle
		glm::vec3 vTest;

		vTest = glm::cross(triangleNormal, vecTriangles[i].p2 - vecTriangles[i].p1);
		if (glm::dot(vTest, intersection - vecTriangles[i].p1) < 0.0f)
			continue;

		vTest = glm::cross(triangleNormal, vecTriangles[i].p3 - vecTriangles[i].p2);
		if (glm::dot(vTest, intersection - vecTriangles[i].p2) < 0.0f)
			continue;

		vTest = glm::cross(triangleNormal, vecTriangles[i].p1 - vecTriangles[i].p3);
		if (glm::dot(vTest, intersection - vecTriangles[i].p1) < 0.0f)
			continue;

		//Reach here: Legit option!
		if (glm::distance(intersection, raycast.origin) < shortestDist)
		{
			shortestDist = glm::distance(intersection, raycast.origin);
			collisionPoint = intersection;
		}
	}

	//print results!
	if (shortestDist == 100000000)
		std::cout << "No collision with green cube!" << std::endl;
	else
		std::cout << "Collision point: (" << collisionPoint.x << ", " << collisionPoint.y << ", " << collisionPoint.z << ")" << std::endl;
}

glm::vec3 MainLevel::TransformPoint(glm::vec3 _p, glm::mat4 _transform)
{
	glm::vec4 transformed = _transform * glm::vec4(_p, 1);
	return glm::vec3(transformed.x, transformed.y, transformed.z);
}

glm::vec3 MainLevel::GetRayDirection(float _x, float _y)
{
	float mouseX = (2.0f * _x) / (float)WINDOW_WIDTH - 1.0f;
	float mouseY = 1.0f - (2.0f * _y) / (float)WINDOW_HEIGHT;

	glm::vec2 normalizedScreenPos = glm::vec2(mouseX, mouseY);

	//Proj space
	glm::vec4 clipCoords = glm::vec4(normalizedScreenPos.x, normalizedScreenPos.y, -1.0f, 1.0f);

	//Eye space
	glm::mat4 invProjMat = glm::inverse(s_instance->m_pCamera->GetProjectionMatrix());
	glm::vec4 eyeCoords = invProjMat * clipCoords;
	eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

	//World space
	glm::mat4 invViewMat = glm::inverse(s_instance->m_pCamera->GetViewMatrix());
	glm::vec4 rayWorld = invViewMat * eyeCoords;
	return glm::normalize(glm::vec3(rayWorld));
}
