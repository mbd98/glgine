#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <shaders.hpp>

GLuint loadShader(const char *vertexPath, const char *fragmentPath)
{
	GLuint vertex, fragment, program;
	int status;
	char log[512];
	std::stringstream vertexCodeBuffer, fragmentCodeBuffer;
	std::string vertexCodeStr, fragmentCodeStr;
	const char *vertexCode, *fragmentCode;
	std::ifstream vertexIn, fragmentIn;
	vertexIn.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentIn.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vertexIn.open(vertexPath);
		vertexCodeBuffer << vertexIn.rdbuf();
		vertexIn.close();
	}
	catch (std::ifstream::failure &ex)
	{
		std::cerr << "Failed to load " << vertexPath << ": " << ex.what() << std::endl;
	}
	try
	{
		fragmentIn.open(fragmentPath);
		fragmentCodeBuffer << fragmentIn.rdbuf();
		fragmentIn.close();
	}
	catch (std::ifstream::failure &ex)
	{
		std::cerr << "Failed to load " << fragmentPath << ": " << ex.what() << std::endl;
	}
	vertexCodeStr = vertexCodeBuffer.str();
	fragmentCodeStr = fragmentCodeBuffer.str();
	vertexCode = vertexCodeStr.c_str();
	fragmentCode = fragmentCodeStr.c_str();

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, nullptr);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, log);
		//throw std::runtime_error(log);
		std::cerr << "Vertex shader error: " << vertexPath << std::endl << log << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, log);
		//throw std::runtime_error(log);
		std::cerr << "Fragment shader error: " << fragmentPath << std::endl << log << std::endl;
	}

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	glGetShaderiv(program, GL_LINK_STATUS, &status);
	if (!status)
	{
		glGetShaderInfoLog(program, 512, nullptr, log);
		//throw std::runtime_error(log);
		std::cerr << "Shader link error: " << log << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return program;
}

void setUniformMat4(GLuint shader, const char *uniform, const glm::mat4 &m)
{
	glUniformMatrix4fv(glGetUniformLocation(shader, uniform), 1, GL_FALSE, glm::value_ptr(m));
}

void setUniformVec2(GLuint shader, const char *uniform, const glm::vec2 &v)
{
	glUniform2fv(glGetUniformLocation(shader, uniform), 1, glm::value_ptr(v));
}

void setUniformVec3(GLuint shader, const char *uniform, const glm::vec3 &v)
{
	glUniform3fv(glGetUniformLocation(shader, uniform), 1, glm::value_ptr(v));
}

void setUniformVec4(GLuint shader, const char *uniform, const glm::vec4 &v)
{
	glUniform4fv(glGetUniformLocation(shader, uniform), 1, glm::value_ptr(v));
}

void setUniformFloat(GLuint shader, const char *uniform, GLfloat f)
{
	glUniform1f(glGetUniformLocation(shader, uniform), f);
}

void setUniformInt(GLuint shader, const char *uniform, GLint i)
{
	glUniform1i(glGetUniformLocation(shader, uniform), i);
}
