#include "terrain.h"
#include "shaderhelper.h"

void Terrain::Initialize(Camera * _pCamera, std::string _heightMapPath, glm::vec3 _position)
{
	m_v3Position = _position;
	m_pCamera = _pCamera;

	const float size = 50;
	GLfloat points[] =
	{
		-size, 0.0f, -size, 0, 0,
		size, 0.0f, -size, 0, 1,
		size, 0.0f, size, 1, 1,
		-size, 0.0f, size, 1, 0
	};

	glPatchParameteri(GL_PATCH_VERTICES, 4); // For quad patches

	// Set up VAO + VBO

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), BUFFER_OFFSET(3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Create shader
	ShaderHelper::CompileAndLinkShaders("terrain_vs.glsl", "terrain_fs.glsl", "terrain_tcs.glsl", "terrain_tes.glsl", m_shader);
	glUseProgram(m_shader);

	// Load heightmap texture
	m_pHeightMap = new Texture(GL_TEXTURE_2D, _heightMapPath);
	if (!m_pHeightMap->load())
	{
		std::cout << "Error Loading Texture from filepath: " << _heightMapPath;
	}
}

void Terrain::Render()
{
	glUseProgram(m_shader);
	m_pHeightMap->bind(GL_TEXTURE0);

	glEnable(GL_DEPTH_TEST);

	glm::mat4 model;
	model = glm::translate(model, m_v3Position);
	glm::mat4 mvp = m_pCamera->GetProjectionMatrix() * m_pCamera->GetViewMatrix() * model;

	GLint mvpLoc = glGetUniformLocation(m_shader, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glBindVertexArray(m_vao);
	glDrawArrays(GL_PATCHES, 0, 4);
	glBindVertexArray(0);
	glDisable(GL_DEPTH_TEST);
}

Terrain::~Terrain()
{
	if (m_pHeightMap)
		delete m_pHeightMap;
}
