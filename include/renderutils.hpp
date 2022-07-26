#pragma once

#include <list>
#include "defaults.hpp"
#include <camera.hpp>

struct Renderable
{
	Renderable();
	Renderable(const Renderable &);
	Renderable(GLuint vao, GLuint sceneShader, GLuint shadowShader, GLuint texture, GLsizei size, GLenum type, const glm::mat4&);
	GLuint vao;
	GLuint sceneShader;
	GLuint shadowShader;
	GLuint texture;
	GLsizei size;
	GLenum type;
	glm::mat4 model;
};

struct VertexData
{
	VertexData();
	VertexData(const VertexData &);
	VertexData(const glm::vec3 &vertexCoord, const glm::vec2 &textureCoord, const glm::vec3 &normal);
	glm::vec3 vertexCoord;
	glm::vec2 textureCoord;
	glm::vec3 normal;
};

extern GLuint createRenderable(Renderable &r, const VertexData *vd, GLsizei vc, GLuint sceneShader, GLuint shadowShader, GLuint texture, GLenum type);
extern void shade(const Renderable &r, const Camera &camera, GLuint depthMap);
extern void render(const Renderable &r, const Camera &cam, GLuint depthMap);
extern GLuint createCube(Renderable &r, GLuint sceneShader, GLuint shadowShader, GLuint texture);
extern GLuint createSquare(Renderable &r, GLuint sceneShader, GLuint shadowShader, GLuint texture);
extern GLuint createGrid(Renderable &, GLuint, int, int, float, float, float);
