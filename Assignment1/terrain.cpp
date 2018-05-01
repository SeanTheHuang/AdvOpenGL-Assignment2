#include "terrain.h"
#include "shaderhelper.h"
#include "soil\SOIL.h"

void Terrain::Initialize(Camera * _pCamera, std::string _heightMapPath, glm::vec3 _position)
{
	m_v3Position = _position;
	m_pCamera = _pCamera;

	GLfloat points[] =
	{
		-m_terrainSize, 0.0f, -m_terrainSize, 0, 0,
		m_terrainSize, 0.0f, -m_terrainSize, 0, 1,
		m_terrainSize, 0.0f, m_terrainSize, 1, 1,
		-m_terrainSize, 0.0f, m_terrainSize, 1, 0
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

	// Create a record of heightmap
	CreateHeightMap(_heightMapPath);
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

	for (int i = 0; i < m_imageHeight; i++)
	{
		delete[] m_heightMap2D[i];
	}

	delete[] m_heightMap2D;
}

float Terrain::GetHeightOnTerrain(glm::vec3 _worldPosition)
{
	// Convert to world position to local to terrain
	// Ignore the y change
	glm::vec3 checkPosition = _worldPosition - m_v3Position;

	if (checkPosition.x < -m_terrainSize || checkPosition.x > m_terrainSize
		|| checkPosition.z < -m_terrainSize || checkPosition.z > m_terrainSize)
	{
		// Default return 0 if not on terrain
		return 0.0f;
	}

	// Get location in %
	float size = m_terrainSize * 2;
	float xPercent = ((checkPosition.x + m_terrainSize) / size);
	float zPercent = ((checkPosition.z + m_terrainSize) / size);

	// Get index in heightmap array
	int xIndex = (int)floorf(xPercent * m_imageWidth);
	int zIndex = (int)floorf(zPercent * m_imageHeight);

	// Return corrosponding value
	return (((float)m_heightMap2D[xIndex][zIndex] / 255.0f) * 15) + m_v3Position.y + 2.5f;
}

void Terrain::CreateHeightMap(std::string _heightMapPath)
{
	unsigned char* image = SOIL_load_image(
		_heightMapPath.c_str(),  // file name
		&m_imageWidth,              // width of the image
		&m_imageHeight,             // height of the image
		0,                        // number of channels
		SOIL_LOAD_RGBA);

	// Create array
	m_heightMap2D = new unsigned char*[m_imageWidth];
	for (size_t i = 0; i < m_imageWidth; i++)
	{
		m_heightMap2D[i] = new unsigned char[m_imageHeight];
	}

	// Record height at each pixel
	for (size_t i = 0; i < m_imageHeight; i++)
	{
		for (size_t j = 0; j < m_imageWidth; j++)
		{
			int index = (j * 4) + (i * m_imageWidth * 4);
			m_heightMap2D[j][i] = image[index];
		}
	}
	

	SOIL_free_image_data(image);
}
