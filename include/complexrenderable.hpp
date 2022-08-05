#pragma once

#include "defaults.hpp"
#include "renderable.hpp"
#include <list>

// Represents a renderable object having position, direction, and scale properties.
class ComplexRenderable
{
private:
	glm::vec3 position;
	glm::vec3 angles;
	glm::vec3 scales;
	// For hierarchical modelling: the parent object we attach to
	ComplexRenderable *parent;
	// The object texture
	GLuint texture;
	bool textured;
	// Tail-recursive helper for computing the hierarchical model matrix
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
	[[nodiscard]] virtual glm::mat4 getWorldTransform() const;
	[[nodiscard]] glm::mat4 getHierarchicalWorldTransform() const;
	[[nodiscard]] GLuint getTexture() const;
	void setTexture(GLuint texture);
	[[nodiscard]] bool isTextured() const;
	virtual void render(GLuint shader) = 0;
};

// Takes a single, simple renderable created from the functions in renderutils.{c,h}pp
class SimpleComplexRenderable : public ComplexRenderable
{
private:
	Renderable *renderable;
public:
	explicit SimpleComplexRenderable(Renderable *renderable);
	~SimpleComplexRenderable() override;
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
	~TimexChar() override;
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

class Reusable : public ComplexRenderable
{
private:
	ComplexRenderable *base;
	std::vector<glm::mat4> transforms;
	uint currentTransform;
public:
	explicit Reusable(ComplexRenderable *base);
	~Reusable();
	void insertTransform(const glm::mat4 &m);
	void setTransforms(const std::vector<glm::mat4> &tv);
	void render(GLuint shader) override;
	[[nodiscard]] glm::mat4 getWorldTransform() const override;
};
