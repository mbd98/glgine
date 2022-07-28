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
protected:
	ComplexRenderable();
	explicit ComplexRenderable(ComplexRenderable *parent);
public:
	[[nodiscard]] glm::mat4 getTranslation() const;
	[[nodiscard]] glm::mat4 getRotation() const;
	[[nodiscard]] glm::mat4 getScaling() const;
	[[nodiscard]] const glm::vec3 &getPosition() const;
	void setPosition(const glm::vec3 &position);
	[[nodiscard]] const glm::vec3 &getAngles() const;
	void setAngles(const glm::vec3 &angles);
	[[nodiscard]] const glm::vec3 &getScales() const;
	void setScales(const glm::vec3 &scales);
	virtual void render() = 0;
};
