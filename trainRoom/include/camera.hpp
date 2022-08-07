#pragma once

#include "defaults.hpp"

class Camera
{
private:
	glm::vec3 yAxis;
	glm::vec3 position;
	float translateSpeed;
	float yaw;
	float pitch;
	float angularSpeed;
	float fov;
	float aspectRatio;
	float near;
	float far;
public:
	Camera();
	Camera(const glm::vec3 &yAxis, const glm::vec3 &position, float translateSpeed, float yaw, float pitch,
	       float angularSpeed, float fov, float aspectRatio, float near, float far);
	[[nodiscard]] const glm::vec3 &getYAxis() const;
	void setYAxis(const glm::vec3 &yAxis);
	[[nodiscard]] const glm::vec3 &getPosition() const;
	void setPosition(const glm::vec3 &position);
	[[nodiscard]] glm::vec3 getDirection() const;
	[[nodiscard]] glm::vec3 getSide() const;
	[[nodiscard]] glm::vec3 getUp() const;
	[[nodiscard]] float getTranslateSpeed() const;
	void setTranslateSpeed(float translateSpeed);
	[[nodiscard]] glm::mat4 getView() const;
	void translateForward(float units);
	void translateRight(float units);
	void translateUp(float units);
	[[nodiscard]] float getYaw() const;
	void setYaw(float yaw);
	[[nodiscard]] float getPitch() const;
	void setPitch(float pitch);
	[[nodiscard]] float getAngularSpeed() const;
	void setAngularSpeed(float angularSpeed);
	void rotateUp(float units);
	void rotateRight(float units);
	[[nodiscard]] float getFov() const;
	void setFov(float fov);
	[[nodiscard]] float getAspectRatio() const;
	void setAspectRatio(float aspectRatio);
	[[nodiscard]] float getNear() const;
	void setNear(float near);
	[[nodiscard]] float getFar() const;
	void setFar(float far);
	[[nodiscard]] glm::mat4 getProjection() const;
};
