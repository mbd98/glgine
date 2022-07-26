#include <vector>
#include <renderutils.hpp>
#include <shaders.hpp>

Renderable::Renderable() : vao(0), sceneShader(0), shadowShader(0), texture(0), size(0), type(0), model(1.0f)
{}

Renderable::Renderable(const Renderable &r) = default;

Renderable::Renderable(GLuint vao, GLuint sceneShader, GLuint shadowShader, GLuint texture, GLsizei size, GLenum type, const glm::mat4 &model)
		: vao(vao), sceneShader(sceneShader), shadowShader(shadowShader), texture(texture), size(size), type(type), model(model)
{}

VertexData::VertexData() : vertexCoord(0.0f), textureCoord(0.0f), normal(0.0f)
{}

VertexData::VertexData(const VertexData &vd) = default;

VertexData::VertexData(const glm::vec3 &vertexCoord, const glm::vec2 &textureCoord, const glm::vec3 &normal)
		: vertexCoord(vertexCoord), textureCoord(textureCoord), normal(normal)
{}

GLuint createRenderable(Renderable &r, const VertexData *vertexData, GLsizei vertexCount, GLuint sceneShader, GLuint shadowShader, GLuint texture, GLenum type)
{
	GLuint vbo;
	glGenVertexArrays(1, &r.vao);
	glBindVertexArray(r.vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(VertexData), vertexData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*) sizeof(glm::vec3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*) (sizeof(glm::vec3) + sizeof(glm::vec2)));
	glEnableVertexAttribArray(2);
	r.size = vertexCount;
	r.sceneShader = sceneShader;
	r.shadowShader = shadowShader;
	r.texture = texture;
	r.type = type;
	glBindVertexArray(0);
	return r.vao;
}

void shade(const Renderable &r, const Camera &camera, GLuint depthMap)
{
	glUseProgram(r.shadowShader);
	setUniformMat4(r.shadowShader, "model", r.model);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	setUniformInt(r.shadowShader, "shadowMap", 1);
	glBindVertexArray(r.vao);
	glDrawArrays(r.type, 0, r.size);
	glBindVertexArray(0);
}

void render(const Renderable &r, const Camera &camera, GLuint depthMap)
{
	glUseProgram(r.sceneShader);
	setUniformMat4(r.sceneShader, "model", r.model);
	setUniformMat4(r.sceneShader, "view", camera.getView());
	setUniformMat4(r.sceneShader, "projection", camera.getProjection());
	setUniformVec3(r.sceneShader, "objColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, r.texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	setUniformInt(r.sceneShader, "textureMap", 0);
	setUniformInt(r.sceneShader, "shadowMap", 1);
	glBindVertexArray(r.vao);
	glDrawArrays(r.type, 0, r.size);
	glBindVertexArray(0);
}

GLuint createCube(Renderable &r, GLuint sceneShader, GLuint shadowShader, GLuint texture)
{
	const glm::vec3 pos = glm::vec3(-0.5f);
	const glm::vec3 delta = glm::vec3(1.0f);
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
	const VertexData vertexData[] = {
			// XY planar face, Z = 0
			VertexData(vertices[0], glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
			VertexData(vertices[2], glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
			VertexData(vertices[4], glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f)),

			VertexData(vertices[6], glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
			VertexData(vertices[4], glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
			VertexData(vertices[2], glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),

			// XY planar face, Z = dZ
			VertexData(vertices[1], glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
			VertexData(vertices[5], glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
			VertexData(vertices[3], glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)),

			VertexData(vertices[7], glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
			VertexData(vertices[3], glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
			VertexData(vertices[5], glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),

			// XZ planar face, Y = 0
			VertexData(vertices[0], glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			VertexData(vertices[4], glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			VertexData(vertices[1], glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),

			VertexData(vertices[5], glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			VertexData(vertices[1], glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			VertexData(vertices[4], glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),

			// XZ planar face, Y = dY
			VertexData(vertices[2], glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			VertexData(vertices[3], glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			VertexData(vertices[6], glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),

			VertexData(vertices[7], glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			VertexData(vertices[6], glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			VertexData(vertices[3], glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),

			// YZ planar face, X = 0
			VertexData(vertices[0], glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),
			VertexData(vertices[1], glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),
			VertexData(vertices[2], glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),

			VertexData(vertices[3], glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),
			VertexData(vertices[2], glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),
			VertexData(vertices[1], glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),

			// YZ planar face, X = dX
			VertexData(vertices[4], glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
			VertexData(vertices[6], glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
			VertexData(vertices[5], glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),

			VertexData(vertices[7], glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
			VertexData(vertices[5], glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
			VertexData(vertices[6], glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
	};
	return createRenderable(r, vertexData, 36, sceneShader, shadowShader, texture, GL_TRIANGLES);
}

GLuint createSquare(Renderable &r, GLuint sceneShader, GLuint shadowShader, GLuint texture)
{
	const VertexData vertexData[] = {
			VertexData(glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			VertexData(glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			VertexData(glm::vec3(0.5f, 0.0f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),

			VertexData(glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			VertexData(glm::vec3(0.5f, 0.0f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			VertexData(glm::vec3(0.5f, 0.0f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
	};
	return createRenderable(r, vertexData, 6, sceneShader, shadowShader, texture, GL_TRIANGLES);
}

GLuint createGrid(Renderable &r, GLuint sceneShader, int columns, int rows, float spacing, float x0, float z0)
{
	float x1 = x0 + (spacing * columns);
	float z1 = z0 + (spacing * rows);
	std::vector<VertexData> vertexData(columns * rows * 4);
	for (int i = 0; i < columns; i++)
	{
		float x = x0 + (spacing * i);
		vertexData.emplace_back(glm::vec3(x, 0.0f, z0), glm::vec2(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		vertexData.emplace_back(glm::vec3(x, 0.0f, z1), glm::vec2(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	for (int i = 0; i < rows; i++)
	{
		float z = z0 + (spacing * i);
		vertexData.emplace_back(glm::vec3(x0, 0.0f, z), glm::vec2(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		vertexData.emplace_back(glm::vec3(x1, 0.0f, z), glm::vec2(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	return createRenderable(r, vertexData.data(), vertexData.size(), sceneShader, 0, 0, GL_LINES);
}
