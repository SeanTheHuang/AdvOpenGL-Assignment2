#include "terrain.h"
#include "shaderhelper.h"

Terrain::Terrain(Camera * _pCamera, std::string _heightMapFilePath)
{
	m_pCamera = _pCamera;
	m_heightMapFilePath = _heightMapFilePath;
}

Terrain::~Terrain()
{
	SOIL_free_image_data(m_image);
}

void Terrain::Initialize(glm::vec3 _position)
{
	m_position = _position;
	ShaderHelper::CompileAndLinkShaders("terrain_vs.glsl", "terrain_fs.glsl", m_shader);

	m_image = SOIL_load_image(
		m_heightMapFilePath.c_str(),  // file name
		&m_imageWidth,              // width of the image
		&m_imageHeight,             // height of the image
		0,                        // number of channels
		SOIL_LOAD_RGB);

	if (m_image == NULL)
	{
		std::cout << "Error: Could not load image: " << m_heightMapFilePath.c_str() << std::endl;
		return;
	}

	SetVAO();
}

float Terrain::GetHeightOnMap(float _x, float _y)
{
	// Outside of map
	if (_x < 0 || _x > 10
		|| _y < 0 || _y > 10)
		return 0;

	return 0;
}

void Terrain::Render()
{
	glEnable(GL_DEPTH_TEST);
	glUseProgram(m_shader);
	glBindVertexArray(m_VAO);


	glm::mat4 position = glm::translate(glm::mat4(1.0), m_position);
	glm::mat4 mvp = m_pCamera->GetProjectionMatrix() * m_pCamera->GetViewMatrix() * position;
	GLint mvpLoc = glGetUniformLocation(m_shader, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_QUADS, 0, m_vertexCount);
	glBindVertexArray(0);
	glDisable(GL_DEPTH_TEST);
}

void Terrain::SetVAO()
{
	std::vector<GLfloat> points;

	// Create 10x10 grid of points
	for (float z = -5; z < 5; z += 0.1f)
	{
		for (float x = -5; x < 5; x += 0.1f)
		{
			// Creating quads

			// Position #1
			points.push_back(x); // x
			points.push_back(0); // y
			points.push_back(z); // z

			// UV coords
			points.push_back((x + 5) / 10.0f);
			points.push_back((z + 5) / 10.0f);

			// Position #2
			points.push_back(x+0.1f); // x
			points.push_back(0); // y
			points.push_back(z); // z

			// UV coords
			points.push_back((x + 5.1f) / 10.0f);
			points.push_back((z + 5) / 10.0f);

			// Position #3
			points.push_back(x+0.1f); // x
			points.push_back(0); // y
			points.push_back(z+0.1f); // z

			// UV coords
			points.push_back((x + 5.1f) / 10.0f);
			points.push_back((z + 5.1f) / 10.0f);

			// Position #4
			points.push_back(x); // x
			points.push_back(0); // y
			points.push_back(z+0.1f); // z

			// UV coords
			points.push_back((x + 5) / 10.0f);
			points.push_back((z + 5.1f) / 10.0f);
		}
	}

	m_vertexCount = points.size();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexCount), points.data(), GL_STATIC_DRAW);

	//Location 0 = index positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	//Location 1 = UV coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), BUFFER_OFFSET(3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}