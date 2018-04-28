#ifndef _SHADER_HELPER__
#define _SHADER_HELPER__

#include <glew\glew.h>
#include <freeglut\freeglut.h>

#include <cstring>
#include <string>

class ShaderHelper {

public:
	static void CompileAndLinkShaders(std::string vertex_shader, std::string fragment_shader, GLuint& program);
	static void CompileAndLinkShaders(std::string vertex_shader, std::string fragment_shader, std::string geometry_shader, GLuint& program);
private:
	ShaderHelper();
	static std::string ReadShaderFileFromResource(const char* pFileName);
	static GLuint CompileVertexShader(const char* shaderCode);
	static GLuint CompileFragmentShader(const char* shaderCode);
	static GLuint CompileShader(GLenum ShaderType, const char* shaderCode);
	static GLuint LinkProgram(GLuint vertexShaderId, GLuint fragmentShaderId);
	static GLuint LinkProgram(GLuint vertexShaderId, GLuint fragmentShaderId, GLuint geometryShaderId);
	static GLint ValidateProgram(GLuint programObjectId);
};

//std::string TAG = "ShaderHelper";
#endif