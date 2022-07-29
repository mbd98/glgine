#pragma once

#include <vector>
#include "defaults.hpp"

struct Vertex
{
	Vertex();
	Vertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &texCoord);
	Vertex(const Vertex &c);
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

class Renderable
{
protected:
	GLuint vao;
	int vertexCount;
	GLenum type;
	Renderable(int vertexCount, GLenum type);
	virtual void doRender() = 0;
public:
	virtual ~Renderable();
	void render();
};

class VertexRenderable : public Renderable
{
protected:
	GLuint vbo;
	void doRender() override;
public:
	VertexRenderable(const std::vector<Vertex> &vertices, GLenum type);
	~VertexRenderable() override;
};

class IndexedRenderable : public VertexRenderable
{
protected:
	GLuint ebo;
	void doRender() override;
public:
	IndexedRenderable(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices, GLenum type);
	~IndexedRenderable() override;
};

