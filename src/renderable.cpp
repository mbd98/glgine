#include <renderable.hpp>

Vertex::Vertex() : position(0.0f), normal(0.0f), texCoord(0.0f)
{}

Vertex::Vertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &texCoord)
		: position(position), normal(normal), texCoord(texCoord)
{}

Vertex::Vertex(const Vertex &c) = default;

Renderable::Renderable(int vertexCount, GLenum type) : vertexCount(vertexCount), type(type)
{
	glGenVertexArrays(1, &vao);
}

Renderable::~Renderable()
{
	glDeleteVertexArrays(1, &vao);
}

void Renderable::render()
{
	glBindVertexArray(vao);
	doRender();
	glBindVertexArray(0);
}

void VertexRenderable::doRender()
{
	glDrawArrays(type, 0, vertexCount);
}

VertexRenderable::VertexRenderable(const std::vector<Vertex> &vertices, GLenum type) : Renderable(vertices.size(), type)
{
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

VertexRenderable::~VertexRenderable()
{
	glDeleteBuffers(1, &vbo);
}

void IndexedRenderable::doRender()
{
	glDrawElements(type, vertexCount, GL_UNSIGNED_INT, nullptr);
}

IndexedRenderable::IndexedRenderable(const std::vector<Vertex> &vertices,
                                     const std::vector<GLuint> &indices,
                                     GLenum type)
		: VertexRenderable(vertices, type)
{
	glBindVertexArray(vao);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
	vertexCount = indices.size();
}

IndexedRenderable::~IndexedRenderable()
{
	glDeleteBuffers(1, &ebo);
}
