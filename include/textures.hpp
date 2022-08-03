#pragma once

#include "defaults.hpp"
#include <vector>
#include <string>

extern GLuint loadTexture(const char*);

struct Texture
{
	GLuint texId;
	std::string type;
};
