#include <complexrenderable.hpp>
#include <renderutils.hpp>
#include <shaders.hpp>
#include <textures.hpp>
#include <utility>

// tail-recursive to prevent stack overflow
glm::mat4 ComplexRenderable::hwtHelper(const glm::mat4 &m) const
{
	glm::mat4 mn = getWorldTransform() * m;
	return parent != nullptr ? parent->hwtHelper(mn) : mn;
}

ComplexRenderable::ComplexRenderable()
		: position(0.0f), angles(0.0f), scales(1.0f), parent(nullptr), texture(0), textured(false)
{}

glm::mat4 ComplexRenderable::getTranslation() const
{
	return glm::translate(glm::mat4(1.0f), position);
}

glm::mat4 ComplexRenderable::getRotation() const
{
	glm::mat4 rx = glm::rotate(glm::mat4(1.0f), angles.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 ry = glm::rotate(glm::mat4(1.0f), angles.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rz = glm::rotate(glm::mat4(1.0f), angles.z, glm::vec3(0.0f, 0.0f, 1.0f));
	return rz * ry * rx;
}

glm::mat4 ComplexRenderable::getScaling() const
{
	return glm::scale(glm::mat4(1.0f), scales);
}

const glm::vec3 &ComplexRenderable::getPosition() const
{
	return position;
}

void ComplexRenderable::setPosition(const glm::vec3 &position)
{
	ComplexRenderable::position = position;
}

const glm::vec3 &ComplexRenderable::getAngles() const
{
	return angles;
}

void ComplexRenderable::setAngles(const glm::vec3 &angles)
{
	ComplexRenderable::angles = angles;
}

const glm::vec3 &ComplexRenderable::getScales() const
{
	return scales;
}

void ComplexRenderable::setScales(const glm::vec3 &scales)
{
	ComplexRenderable::scales = scales;
}

ComplexRenderable *ComplexRenderable::getParent() const
{
	return parent;
}

void ComplexRenderable::setParent(ComplexRenderable *parent)
{
	ComplexRenderable::parent = parent;
}

glm::mat4 ComplexRenderable::getWorldTransform() const
{
	return getTranslation() * getRotation() * getScaling();
}

glm::mat4 ComplexRenderable::getHierarchicalWorldTransform() const
{
	return hwtHelper(glm::mat4(1.0f));
}

GLuint ComplexRenderable::getTexture() const
{
	return texture;
}

void ComplexRenderable::setTexture(GLuint texture)
{
	ComplexRenderable::texture = texture;
	textured = true;
}

bool ComplexRenderable::isTextured() const
{
	return textured;
}

ComplexRenderable::~ComplexRenderable() = default;

SimpleComplexRenderable::SimpleComplexRenderable(Renderable *renderable) : ComplexRenderable(), renderable(renderable)
{}

SimpleComplexRenderable::~SimpleComplexRenderable()
{
	delete renderable;
}

void SimpleComplexRenderable::render(GLuint shader)
{
	setUniformMat4(shader, MODEL, getHierarchicalWorldTransform());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, getTexture());
	setUniformInt(shader, TEXTURE_MAP, 0);
	setUniformInt(shader, TEXTURE_PRESENT, isTextured() ? 1 : 0);
	renderable->render();
}

Skateboard::Skateboard(ComplexRenderable *character) : ComplexRenderable(), character(character)
{
	GLuint wheelTex = loadTexture("assets/textures/blackleather.jpg");
	plank = new SimpleComplexRenderable(createCuboid(glm::vec3(-4.0f, 1.0f, -2.0f), glm::vec3(8.0f, 0.2f, 4.0f)));
	plank->setParent(this);
	plank->setTexture(loadTexture("assets/textures/metal.jpg"));
	for (int i = 0; i < 4; i++)
	{
		wheels[i] = new SimpleComplexRenderable(createCuboid(glm::vec3(-0.5f), glm::vec3(1.0f)));
		wheels[i]->setParent(plank);
		wheels[i]->setTexture(wheelTex);
	}
	wheels[0]->setPosition(wheels[0]->getPosition() + glm::vec3(-1.5f, 0.5f, -1.5f));
	wheels[1]->setPosition(wheels[1]->getPosition() + glm::vec3(-1.5f, 0.5f, 1.5f));
	wheels[2]->setPosition(wheels[2]->getPosition() + glm::vec3(1.5f, 0.5f, -1.5f));
	wheels[3]->setPosition(wheels[3]->getPosition() + glm::vec3(1.5f, 0.5f, 1.5f));
	character->setParent(this);
	character->setPosition(glm::vec3(0.0f, 1.2f, 0.0f));
}

Skateboard::~Skateboard()
{
	delete plank;
	for (auto &wheel : wheels)
		delete wheel;
	delete character;
}

void Skateboard::render(GLuint shader)
{
	setUniformVec4(shader, OBJECT_COLOR, glm::vec4(170.0f/255.0f, 169.0f/255.0f, 173.0f/255.0f, 1.0f));
	setUniformFloat(shader, SHINE, 256.0f);
	plank->render(shader);
	setUniformFloat(shader, SHINE, 64.0f);
	setUniformVec4(shader, OBJECT_COLOR, glm::vec4(glm::vec3(0.0f),1.0f));
	for (auto &wheel : wheels)
		wheel->render(shader);
	setUniformFloat(shader, SHINE, 16.0f);
	setUniformVec4(shader, OBJECT_COLOR, glm::vec4(0.0f, 0.0f, 0.8f, 0.5f));
	if (character != nullptr)
		character->render(shader);
	setUniformFloat(shader, SHINE, 32.0f);
}

TimexChar::TimexChar(bool doBottom,
                     bool doBottomLeft,
                     bool doBottomRight,
                     bool doMiddle,
                     bool doTopLeft,
                     bool doTopRight,
                     bool doTop)
		: bottom(nullptr),
		  bottomLeft(nullptr),
		  bottomRight(nullptr),
		  middle(nullptr),
		  topLeft(nullptr),
		  topRight(nullptr),
		  top(nullptr)
{
	if (doBottom)
	{
		bottom = new SimpleComplexRenderable(createCuboid(glm::vec3(0.2f, 0.0f, 0.0f), glm::vec3(0.6f, 0.2f, 0.2f)));
		bottom->setParent(this);
	}
	if (doBottomLeft)
	{
		bottomLeft = new SimpleComplexRenderable(
				createCuboid(glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(0.2f, 0.6f, 0.2f)));
		bottomLeft->setParent(this);
	}
	if (doBottomRight)
	{
		bottomRight = new SimpleComplexRenderable(
				createCuboid(glm::vec3(0.8f, 0.2f, 0.0f), glm::vec3(0.2f, 0.6f, 0.2f)));
		bottomRight->setParent(this);
	}
	if (doMiddle)
	{
		middle = new SimpleComplexRenderable(createCuboid(glm::vec3(0.2f, 0.8f, 0.0f), glm::vec3(0.6f, 0.2f, 0.2f)));
		middle->setParent(this);
	}
	if (doTopLeft)
	{
		topLeft = new SimpleComplexRenderable(createCuboid(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.2f, 0.6f, 0.2f)));
		topLeft->setParent(this);
	}
	if (doTopRight)
	{
		topRight = new SimpleComplexRenderable(createCuboid(glm::vec3(0.8f, 1.0f, 0.0f), glm::vec3(0.2f, 0.6f, 0.2f)));
		topRight->setParent(this);
	}
	if (doTop)
	{
		top = new SimpleComplexRenderable(createCuboid(glm::vec3(0.2f, 1.6f, 0.0f), glm::vec3(0.6f, 0.2f, 0.2f)));
		top->setParent(this);
	}
}

TimexChar::~TimexChar()
{
	delete bottom;
	delete bottomLeft;
	delete bottomRight;
	delete middle;
	delete topLeft;
	delete topRight;
	delete top;
}

void TimexChar::render(GLuint shader)
{
	if (bottom)
		bottom->render(shader);
	if (bottomLeft)
		bottomLeft->render(shader);
	if (bottomRight)
		bottomRight->render(shader);
	if (middle)
		middle->render(shader);
	if (topLeft)
		topLeft->render(shader);
	if (topRight)
		topRight->render(shader);
	if (top)
		top->render(shader);
}

Multi::Multi(std::vector<ComplexRenderable *> renderables) : renderables(std::move(renderables))
{
	for (auto &r : Multi::renderables)
	{
		r->setParent(this);
	}
}

void Multi::render(GLuint shader)
{
	for (auto &r : renderables)
	{
		r->render(shader);
	}
}
