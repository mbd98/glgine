#pragma once

#include <list>
#include "defaults.hpp"
#include "renderable.hpp"

/*
extern GLuint createRenderable(Renderable &r, const VertexData *vd, GLsizei vc, GLuint sceneShader, GLuint shadowShader, GLuint texture, GLenum type);
extern void shade(const Renderable &r, const Camera &camera, GLuint depthMap);
extern void render(const Renderable &r, const Camera &cam, GLuint depthMap);
 */
extern Renderable * createCuboid(const glm::vec3 &pos, const glm::vec3 &delta);
extern Renderable * createSquare();
extern Renderable * createGrid(int columns, int rows, float spacing, float x0, float z0);
