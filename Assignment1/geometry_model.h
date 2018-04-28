#pragma once

#include "include.h"
#include "camera.h"

class GeometryModel
{
public:
	GeometryModel() = delete;
	GeometryModel(Camera* _pCamera, glm::vec3 _position);
	void Render();

private:
	Camera *m_pCamera;
	glm::vec3 m_position;

	GLuint m_vao, m_vbo, m_shader;
};