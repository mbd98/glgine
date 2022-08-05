#pragma once

#include "complexrenderable.hpp"
#include "model.hpp"

class Tracks : public ComplexRenderable
{
private:
	Reusable *segments[4];
public:
	Tracks();
	~Tracks();
	void render(GLuint shader) override;
};
