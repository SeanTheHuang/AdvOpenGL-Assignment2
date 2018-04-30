#include "framebuffer.h"
#include "shaderhelper.h"

void FrameBuffer::Initialize()
{
	m_bEdgeDetection = true;

	// Create Framebuffer
	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	// Generate render texture
	glGenTextures(1, &m_renderTexture);
	glBindTexture(GL_TEXTURE_2D, m_renderTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Attach texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTexture, 0);
	
	// Generate RBO
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR: Framebuffer could not be completed!" << std::endl;

	// Create shader and Set VAO
	ShaderHelper::CompileAndLinkShaders("framebuffer_vs.glsl", "framebuffer_fs.glsl", m_shader);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(frameVerts), &frameVerts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

	glBindVertexArray(0);
}

void FrameBuffer::StartRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

}

void FrameBuffer::DisplayBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_shader);

	GLint ifEdgeLoc = glGetUniformLocation(m_shader, "ifEdgeDetection");
	glUniform1f(ifEdgeLoc, (m_bEdgeDetection) ? 1.0f : 0.0f);

	glBindVertexArray(m_vao);
	glBindTexture(GL_TEXTURE_2D, m_renderTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void FrameBuffer::Update()
{
	if (Input::Instance().GetKeyDown('e'))
		m_bEdgeDetection = !m_bEdgeDetection;
}