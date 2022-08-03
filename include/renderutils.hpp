#pragma once

#include <list>
#include "defaults.hpp"
#include "renderable.hpp"

extern Renderable * createCuboid(const glm::vec3 &pos, const glm::vec3 &delta);
extern Renderable * createSquare();
extern Renderable * createGrid(int columns, int rows, float spacing, float x0, float z0);
