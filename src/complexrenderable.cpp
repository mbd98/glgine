#include <complexrenderable.hpp>

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
