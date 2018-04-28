#pragma once

#include "include.h"

static GLfloat frameVerts[]
{
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};

class FrameBuffer
{
public:
	void Initialize();
	void StartRender();
	void DisplayBuffer();
	void Update();
private:
	GLuint m_renderTexture, m_rbo, m_vao, m_vbo, m_shader, m_frameBuffer;
	bool m_bEdgeDetection;
};