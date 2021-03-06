#pragma once

#include "include.h"
#include "camera.h"
#include "texture.h"

class Terrain
{
	// Functions
public:
	void Initialize(Camera* _pCamera, std::string _heightMapPath, glm::vec3 _position);
	void Render();
	~Terrain();
	float GetHeightOnTerrain(glm::vec3 _worldPosition);
private:
	void CreateHeightMap(std::string _heightMapPath);

	// Variables
private:
	const float m_terrainSize = 50;
	Camera* m_pCamera;
	Texture* m_pHeightMap;
	glm::vec3 m_v3Position;
	GLuint m_shader, m_vao, m_vbo, m_ebo;

	unsigned char** m_heightMap2D;
	int m_imageWidth, m_imageHeight;
};