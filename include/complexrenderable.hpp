#pragma once

#include "defaults.hpp"
#include "renderable.hpp"

class ComplexRenderable
{
private:
	glm::vec3 position;
	glm::vec3 angles;
	glm::vec3 scales;
	ComplexRenderable *parent;
	GLuint texture;
	bool textured;
	[[nodiscard]] glm::mat4 hwtHelper(const glm::mat4 &m) const;
protected:
	ComplexRenderable();
public:
	virtual ~ComplexRenderable();
	[[nodiscard]] glm::mat4 getTranslation() const;
	[[nodiscard]] glm::mat4 getRotation() const;
	[[nodiscard]] glm::mat4 getScaling() const;
	[[nodiscard]] const glm::vec3 &getPosition() const;
	void setPosition(const glm::vec3 &position);
	[[nodiscard]] const glm::vec3 &getAngles() const;
	void setAngles(const glm::vec3 &angles);
	[[nodiscard]] const glm::vec3 &getScales() const;
	void setScales(const glm::vec3 &scales);
	[[nodiscard]] ComplexRenderable *getParent() const;
	void setParent(ComplexRenderable *parent);
	[[nodiscard]] glm::mat4 getWorldTransform() const;
	[[nodiscard]] glm::mat4 getHierarchicalWorldTransform() const;
	[[nodiscard]] GLuint getTexture() const;
	void setTexture(GLuint texture);
	bool isTextured() const;
	virtual void render(GLuint shader) = 0;
};

class SimpleComplexRenderable : public ComplexRenderable
{
private:
	Renderable *renderable;
public:
	explicit SimpleComplexRenderable(Renderable *renderable);
	~SimpleComplexRenderable();
	void render(GLuint shader) override;
};

class Skateboard : public ComplexRenderable
{
private:
	SimpleComplexRenderable *plank;
	SimpleComplexRenderable *wheels[4];
	ComplexRenderable *character;
public:
	explicit Skateboard(ComplexRenderable *character);
	~Skateboard();
	void render(GLuint shader) override;
};

class TimexChar : public ComplexRenderable
{
private:
	ComplexRenderable *bottom;
	ComplexRenderable *bottomLeft;
	ComplexRenderable *bottomRight;
	ComplexRenderable *middle;
	ComplexRenderable *topLeft;
	ComplexRenderable *topRight;
	ComplexRenderable *top;
public:
	TimexChar(bool doBottom, bool doBottomLeft, bool doBottomRight, bool doMiddle, bool doTopLeft, bool doTopRight, bool doTop);
	~TimexChar();
	void render(GLuint shader) override;
};

class Multi : public ComplexRenderable
{
private:
	std::vector<ComplexRenderable*> renderables;
public:
	explicit Multi(std::vector<ComplexRenderable*> renderables);
	void render(GLuint shader) override;
};
