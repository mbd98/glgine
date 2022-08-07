#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <shaders.hpp>

const char * const MODEL = "model";
const char * const VIEW = "view";
const char * const PROJECTION = "projection";
const char * const LIGHT0 = "light0";
const char * const LIGHT1 = "light1";
const char * const LIGHT2 = "light2";
const char * const TEXTURE_DIFFUSE0 = "texture_diffuse0";
const char * const TEXTURE_SPECULAR0 = "texture_specular0";
const char * const SHADOW_MAP = "shadowMap";
//const char * const LIGHT_COLOR = "lightColor";
const char * const LIGHT0_POSITION = "lights[0].position";
const char * const LIGHT0_AMBIENT = "lights[0].ambient";
const char * const LIGHT0_DIFFUSE = "lights[0].diffuse";
const char * const LIGHT0_SPECULAR = "lights[0].specular";
const char * const LIGHT0_DIRECTION = "lights[0].direction";
const char * const LIGHT1_POSITION = "lights[1].position";
const char * const LIGHT1_AMBIENT = "lights[1].ambient";
const char * const LIGHT1_DIFFUSE = "lights[1].diffuse";
const char * const LIGHT1_SPECULAR = "lights[1].specular";
const char * const LIGHT1_DIRECTION = "lights[1].direction";
const char * const LIGHT2_POSITION = "lights[2].position";
const char * const LIGHT2_AMBIENT = "lights[2].ambient";
const char * const LIGHT2_DIFFUSE = "lights[2].diffuse";
const char * const LIGHT2_SPECULAR = "lights[2].specular";
const char * const LIGHT2_DIRECTION = "lights[2].direction";
const char * const AMBIENT_STRENGTH = "ambientStrength";
const char * const DIFFUSE_STRENGTH = "diffuseStrength";
const char * const SPECULAR_STRENGTH = "specularStrength";
const char * const VIEW_POSITION = "viewPosition";
const char * const DO_LIGHTING = "doLighting";
const char * const LIGHT_INNER = "light_cutoff_inner";
const char * const LIGHT_OUTER = "light_cutoff_outer";
const char * const SHINE = "shine";

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

	std::cerr << "Loading vertex shader " << vertexPath << std::endl;

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

	std::cerr << "Loading fragment shader " << fragmentPath << std::endl;

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
