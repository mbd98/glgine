#include <camera.hpp>

Camera::Camera() : yAxis(0.0f, 1.0f, 0.0f),
                   position(0.0f),
                   translateSpeed(1.0f),
                   yaw(glm::radians(-90.0f)),
                   pitch(0.0f),
                   angularSpeed(glm::radians(5.0f)),
                   fov(90.0),
                   aspectRatio(1.777778f),
                   near(0.01f),
                   far(100.0f)
{}

Camera::Camera(const glm::vec3 &yAxis,
               const glm::vec3 &position,
               float translateSpeed,
               float yaw,
               float pitch,
               float angularSpeed,
               float fov,
               float aspectRatio,
               float near,
               float far)
		: yAxis(yAxis),
		  position(position),
		  translateSpeed(translateSpeed),
		  yaw(yaw),
		  pitch(pitch),
		  angularSpeed(angularSpeed),
		  fov(fov),
		  aspectRatio(aspectRatio),
		  near(near),
		  far(far)
{}

const glm::vec3 &Camera::getPosition() const
{
	return position;
}

void Camera::setPosition(const glm::vec3 &position)
{
	Camera::position = position;
}

const glm::vec3 &Camera::getYAxis() const
{
	return yAxis;
}

void Camera::setYAxis(const glm::vec3 &yAxis)
{
	Camera::yAxis = yAxis;
}

glm::vec3 Camera::getDirection() const
{
	return glm::normalize(glm::vec3(glm::cos(yaw) * glm::cos(pitch), glm::sin(pitch), -glm::sin(yaw) * glm::cos(pitch)));
}

glm::vec3 Camera::getSide() const
{
	return glm::normalize(glm::cross(getDirection(), getYAxis()));
}

glm::vec3 Camera::getUp() const
{
	return glm::normalize(glm::cross(getSide(), getDirection()));
}

float Camera::getTranslateSpeed() const
{
	return translateSpeed;
}

void Camera::setTranslateSpeed(float translateSpeed)
{
	Camera::translateSpeed = translateSpeed;
}

glm::mat4 Camera::getView() const
{
	return glm::lookAt(getPosition(), getPosition() + getDirection(), getUp());
}

void Camera::translateForward(float units)
{
	position += getTranslateSpeed() * units * getDirection();
}

void Camera::translateRight(float units)
{
	position += getTranslateSpeed() * units * getSide();
}

void Camera::translateUp(float units)
{
	position += getTranslateSpeed() * units * getUp();
}

float Camera::getYaw() const
{
	return yaw;
}

void Camera::setYaw(float yaw)
{
	Camera::yaw = yaw;
}

float Camera::getPitch() const
{
	return pitch;
}

void Camera::setPitch(float pitch)
{
	Camera::pitch = pitch;
}

float Camera::getAngularSpeed() const
{
	return angularSpeed;
}

void Camera::setAngularSpeed(float angularSpeed)
{
	Camera::angularSpeed = angularSpeed;
}

void Camera::rotateUp(float units)
{
	pitch -= angularSpeed * units;
	pitch = glm::max(glm::radians(-89.0f), glm::min(glm::radians(89.0f), pitch));
}

void Camera::rotateRight(float units)
{
	yaw -= angularSpeed * units;
	if (yaw > glm::radians(360.0f))
		yaw += glm::radians(-360.0f);
	else if (yaw < glm::radians(-360.0f))
		yaw += glm::radians(360.0f);
}

float Camera::getFov() const
{
	return fov;
}

void Camera::setFov(float fov)
{
	Camera::fov = fov;
}

float Camera::getAspectRatio() const
{
	return aspectRatio;
}

void Camera::setAspectRatio(float aspectRatio)
{
	Camera::aspectRatio = aspectRatio;
}

float Camera::getNear() const
{
	return near;
}

void Camera::setNear(float near)
{
	Camera::near = near;
}

float Camera::getFar() const
{
	return far;
}

void Camera::setFar(float far)
{
	Camera::far = far;
}

glm::mat4 Camera::getProjection() const
{
	return glm::perspective(fov, aspectRatio, near, far);
}
