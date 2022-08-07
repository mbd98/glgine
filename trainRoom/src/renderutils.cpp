#include <vector>
#include <renderutils.hpp>

Renderable * createCuboid(const glm::vec3 &pos, const glm::vec3 &delta)
{
	const glm::vec3 vertices[] = {
			pos,
			pos + glm::vec3(0.0f, 0.0f, delta.z),
			pos + glm::vec3(0.0f, delta.y, 0.0f),
			pos + glm::vec3(0.0f, delta.y, delta.z),
			pos + glm::vec3(delta.x, 0.0f, 0.0f),
			pos + glm::vec3(delta.x, 0.0f, delta.z),
			pos + glm::vec3(delta.x, delta.y, 0.0f),
			pos + delta,
	};
	const std::vector<Vertex> vertexData = {
			// XY planar face, Z = 0
			Vertex(vertices[0], glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
			Vertex(vertices[2], glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
			Vertex(vertices[4], glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)),

			Vertex(vertices[6], glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
			Vertex(vertices[4], glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)),
			Vertex(vertices[2], glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),

			// XY planar face, Z = dZ
			Vertex(vertices[1], glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
			Vertex(vertices[5], glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
			Vertex(vertices[3], glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)),

			Vertex(vertices[7], glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
			Vertex(vertices[3], glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
			Vertex(vertices[5], glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),

			// XZ planar face, Y = 0
			Vertex(vertices[0], glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
			Vertex(vertices[4], glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
			Vertex(vertices[1], glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),

			Vertex(vertices[5], glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
			Vertex(vertices[1], glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
			Vertex(vertices[4], glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),

			// XZ planar face, Y = dY
			Vertex(vertices[2], glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
			Vertex(vertices[3], glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
			Vertex(vertices[6], glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),

			Vertex(vertices[7], glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
			Vertex(vertices[6], glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
			Vertex(vertices[3], glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),

			// YZ planar face, X = 0
			Vertex(vertices[0], glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
			Vertex(vertices[1], glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
			Vertex(vertices[2], glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),

			Vertex(vertices[3], glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
			Vertex(vertices[2], glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
			Vertex(vertices[1], glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),

			// YZ planar face, X = dX
			Vertex(vertices[4], glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
			Vertex(vertices[6], glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
			Vertex(vertices[5], glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),

			Vertex(vertices[7], glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
			Vertex(vertices[5], glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
			Vertex(vertices[6], glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
	};
	return new VertexRenderable(vertexData, GL_TRIANGLES);
}

Renderable * createSquare()
{
	const std::vector<Vertex> vertexData = {
			Vertex(glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
			Vertex(glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),

			Vertex(glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
			Vertex(glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
			Vertex(glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
	};
	
	return new VertexRenderable(vertexData, GL_TRIANGLES);
}

Renderable * createGrid(int columns, int rows, float spacing, float x0, float z0)
{
	float x1 = x0 + (spacing * columns);
	float z1 = z0 + (spacing * rows);
	std::vector<Vertex> vertexData(columns * rows * 4);
	for (int i = 0; i < columns; i++)
	{
		float x = x0 + (spacing * i);
		vertexData.emplace_back(glm::vec3(x, 0.0f, z0), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f));
		vertexData.emplace_back(glm::vec3(x, 0.0f, z1), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f));
	}
	for (int i = 0; i < rows; i++)
	{
		float z = z0 + (spacing * i);
		vertexData.emplace_back(glm::vec3(x0, 0.0f, z), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f));
		vertexData.emplace_back(glm::vec3(x1, 0.0f, z), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f));
	}
	return new VertexRenderable(vertexData, GL_LINES);
}
