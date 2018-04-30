#include "ShaderHelper.h"

#include <iostream>
#include <fstream>
#include <conio.h>

ShaderHelper::ShaderHelper() {

}

std::string ShaderHelper::ReadShaderFileFromResource(const char* pFileName) {
	std::string outFile;
	try {
		std::ifstream f(pFileName);
		std::string line;
		while (std::getline(f, line)) {
			outFile.append(line);
			outFile.append("\n");
		}
		f.close();
	}
	catch (std::ifstream::failure e) {
		std::cerr << "Exception opening/reading/closing file\n";
	}
	return outFile;
}

GLuint ShaderHelper::CompileVertexShader(const char* shaderCode) {
	return CompileShader(GL_VERTEX_SHADER, shaderCode);
}

GLuint ShaderHelper::CompileFragmentShader(const char* shaderCode) {
	return CompileShader(GL_FRAGMENT_SHADER, shaderCode);
}

GLuint ShaderHelper::CompileShader(GLenum ShaderType, const char* shaderCode) {
	const  GLuint shaderObjectId = glCreateShader(ShaderType);
	if (shaderObjectId == 0) {
		std::cout << "Error creating shader type " << ShaderType << std::endl;
		_getch();
		exit(0);
	}
	const GLchar* p[1];
	p[0] = shaderCode;
	GLint Lengths[1];
	Lengths[0] = strlen(shaderCode);

	glShaderSource(shaderObjectId, 1, p, Lengths);
	glCompileShader(shaderObjectId);
	GLint compileStatus;
	glGetShaderiv(shaderObjectId, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shaderObjectId, 1024, NULL, InfoLog);
		std::cout << "Error compiling shader type" << ShaderType << std::endl << InfoLog << std::endl;
		_getch();
		exit(1);
	}

	return shaderObjectId;
}

GLuint ShaderHelper::LinkProgram(GLuint vertexShaderId, GLuint fragmentShaderId) {
	GLint linkStatus = 0;
	GLchar ErrorLog[1024] = { 0 };
	const GLuint programObjectId = glCreateProgram();

	if (programObjectId == 0) {
		std::cout << "Error creating shader program " << std::endl;
		_getch();
		exit(1);
	}

	glAttachShader(programObjectId, vertexShaderId);
	glAttachShader(programObjectId, fragmentShaderId);
	glLinkProgram(programObjectId);

	glGetProgramiv(programObjectId, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == 0) {
		glGetProgramInfoLog(programObjectId, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << "Error linking shader program: " << std::endl << ErrorLog << std::endl;
		_getch();
		exit(1);
	}
	return programObjectId;
}

GLuint ShaderHelper::LinkProgram(GLuint vertexShaderId, GLuint fragmentShaderId, GLuint geometryShaderId) {
	GLint linkStatus = 0;
	GLchar ErrorLog[1024] = { 0 };
	const GLuint programObjectId = glCreateProgram();

	if (programObjectId == 0) {
		std::cout << "Error creating shader program " << std::endl;
		_getch();
		exit(1);
	}

	glAttachShader(programObjectId, vertexShaderId);
	glAttachShader(programObjectId, fragmentShaderId);
	glAttachShader(programObjectId, geometryShaderId);
	glLinkProgram(programObjectId);

	glGetProgramiv(programObjectId, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == 0) {
		glGetProgramInfoLog(programObjectId, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << "Error linking shader program: " << std::endl << ErrorLog << std::endl;
		_getch();
		exit(1);
	}
	return programObjectId;
}

GLuint ShaderHelper::LinkProgram(GLuint vertexShaderId, GLuint fragmentShaderId, GLuint tcsId, GLuint tesId)
{
	GLint linkStatus = 0;
	GLchar ErrorLog[1024] = { 0 };
	const GLuint programObjectId = glCreateProgram();

	if (programObjectId == 0) {
		std::cout << "Error creating shader program " << std::endl;
		_getch();
		exit(1);
	}

	glAttachShader(programObjectId, vertexShaderId);
	glAttachShader(programObjectId, fragmentShaderId);
	glAttachShader(programObjectId, tcsId);
	glAttachShader(programObjectId, tesId);
	glLinkProgram(programObjectId);

	glGetProgramiv(programObjectId, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == 0) {
		glGetProgramInfoLog(programObjectId, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << "Error linking shader program: " << std::endl << ErrorLog << std::endl;
		_getch();
		exit(1);
	}
	return programObjectId;
}

GLint ShaderHelper::ValidateProgram(GLuint programObjectId) {
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	glValidateProgram(programObjectId);
	glGetProgramiv(programObjectId, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(programObjectId, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << "Invalid shader program: " << std::endl << ErrorLog;
		_getch();
		exit(1);
	}
	return Success;
}

void ShaderHelper::CompileAndLinkShaders(std::string vertex_shader, std::string fragment_shader, GLuint& program) {
	std::string vertexShaderSource = ShaderHelper::ReadShaderFileFromResource(vertex_shader.c_str());
	std::string fragmentShaderSource = ShaderHelper::ReadShaderFileFromResource(fragment_shader.c_str());
	GLuint vertexShader = ShaderHelper::CompileVertexShader(vertexShaderSource.c_str());
	GLuint fragmentShader = ShaderHelper::CompileFragmentShader(fragmentShaderSource.c_str());
	program = ShaderHelper::LinkProgram(vertexShader, fragmentShader);
	ShaderHelper::ValidateProgram(program);
}

void ShaderHelper::CompileAndLinkShaders(std::string vertex_shader, std::string fragment_shader,  std::string geomtry_shader, GLuint& program) {
	std::string vertexShaderSource = ShaderHelper::ReadShaderFileFromResource(vertex_shader.c_str());
	std::string fragmentShaderSource = ShaderHelper::ReadShaderFileFromResource(fragment_shader.c_str());
	std::string geometryShaderSource = ShaderHelper::ReadShaderFileFromResource(geomtry_shader.c_str());
	GLuint vertexShader = ShaderHelper::CompileVertexShader(vertexShaderSource.c_str());
	GLuint fragmentShader = ShaderHelper::CompileFragmentShader(fragmentShaderSource.c_str());
	GLuint geomtryShader = CompileShader(GL_GEOMETRY_SHADER, geometryShaderSource.c_str());
	program = ShaderHelper::LinkProgram(vertexShader, fragmentShader, geomtryShader);
	ShaderHelper::ValidateProgram(program);
}

void ShaderHelper::CompileAndLinkShaders(std::string vertex_shader, std::string fragment_shader, std::string tcs_shader, std::string tes_shader, GLuint & program)
{
	std::string vertexShaderSource = ShaderHelper::ReadShaderFileFromResource(vertex_shader.c_str());
	std::string fragmentShaderSource = ShaderHelper::ReadShaderFileFromResource(fragment_shader.c_str());
	std::string tcsSource = ShaderHelper::ReadShaderFileFromResource(tcs_shader.c_str());
	std::string tesSource = ShaderHelper::ReadShaderFileFromResource(tes_shader.c_str());
	GLuint vertexShader = ShaderHelper::CompileVertexShader(vertexShaderSource.c_str());
	GLuint fragmentShader = ShaderHelper::CompileFragmentShader(fragmentShaderSource.c_str());
	GLuint tcsShader = CompileShader(GL_TESS_CONTROL_SHADER, tcsSource.c_str());
	GLuint tesShader = CompileShader(GL_TESS_EVALUATION_SHADER, tesSource.c_str());
	program = ShaderHelper::LinkProgram(vertexShader, fragmentShader, tcsShader, tesShader);
	ShaderHelper::ValidateProgram(program);
}
