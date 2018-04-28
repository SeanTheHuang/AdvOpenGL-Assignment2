#pragma once

#include "include.h"
#include <soil\SOIL.h>
#include "camera.h"

class Terrain
{
	// Functions
public:

	Terrain() = delete;
	Terrain(Camera* _pCamera, std::string _heightMapFilePath);
	~Terrain();

	void Initialize(glm::vec3 _position);
	float GetHeightOnMap(float _x, float _y);
	void Render();

private:
	void SetVAO();

	// Variables
private:
	Camera *m_pCamera;
	std::string m_heightMapFilePath;
	int m_vertexCount;
	
	unsigned char* m_image;
	int m_imageWidth, m_imageHeight;

	glm::vec3 m_position;

	GLuint m_shader, m_VAO, m_VBO;
};