#include "geometry_model.h"
#include "shaderhelper.h"

GLfloat points[] =
{
	0, 0, 0, // Position
	0.9f, 0.7f, 0.3f // Colour
};

GeometryModel::GeometryModel(Camera* _pCamera, glm::vec3 _position)
{
	m_pCamera = _pCamera;
	m_position = _position;

	ShaderHelper::CompileAndLinkShaders("star_vs.glsl", "star_fs.glsl", "star_gs.glsl", m_shader);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindVertexArray(0);
}

void GeometryModel::Render()
{
	glEnable(GL_DEPTH_TEST);
	glUseProgram(m_shader);

	glm::mat4 model;
	model = glm::translate(model, m_position);
	glm::mat4 mvp = m_pCamera->GetProjectionMatrix() * m_pCamera->GetViewMatrix() * model;

	GLint mvpLoc = glGetUniformLocation(m_shader, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(m_vao);
	glLineWidth(4);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
	glDisable(GL_DEPTH_TEST);
}

