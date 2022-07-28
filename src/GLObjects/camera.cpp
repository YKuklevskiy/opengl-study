#include "GLObjects/camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 initialPosition, float initialYaw, float initialPitch)
	:_position(initialPosition), _yaw(initialYaw), _pitch(initialPitch) { }

void Camera::handleMovement(glm::vec3 offset, float speedModifier)
{
	if (!_controlsEnabled) return;

	float modifier = _speed * speedModifier;

	// transform offset to be relative to camera
	glm::mat4 cameraSpaceMatrix = glm::mat4(1.0f);
	cameraSpaceMatrix = glm::rotate(cameraSpaceMatrix, glm::radians(_yaw + 90.0f), glm::vec3(0, -1, 0));
	cameraSpaceMatrix = glm::rotate(cameraSpaceMatrix, glm::radians(_pitch), glm::vec3(1, 0, 0));
	cameraSpaceMatrix = glm::scale(cameraSpaceMatrix, glm::vec3(1, 1, -1)); // right handed coordinate system in opengl
	offset = cameraSpaceMatrix * glm::vec4(offset, 0.0f);

	_position += offset * modifier;
}

float clamp(float value, float leftBorder, float rightBorder)
{
	return (value < leftBorder) ? leftBorder :
		((value > rightBorder) ? rightBorder : value);
}

void Camera::handleRotation(float xoffset, float yoffset, float sensitivityModifier)
{
	if (!_controlsEnabled) return;

	const float pitchBorder = 88.0f;

	float modifier = _sensitivity * sensitivityModifier;

	_yaw = _yaw + xoffset * modifier;
	_pitch = clamp(_pitch + yoffset * modifier, -pitchBorder, pitchBorder);
}

const glm::vec3& const Camera::getPosition() const
{
	return _position;
}

const glm::vec3 Camera::getDirectionVector() const
{
	return glm::normalize(
		glm::vec3(
		cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)),
		sin(glm::radians(_pitch)),
		sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))
	));
}

void Camera::enableMovement()
{
	_controlsEnabled = true;
}

void Camera::disableMovement()
{
	_controlsEnabled = false;
}

#pragma region Parameter Getters and Setters
const float Camera::getYaw() const
{
	return _yaw;
}

const float Camera::getPitch() const
{
	return _pitch;
}

void Camera::setSpeed(float speed)
{
	_speed = speed;
}

const float Camera::getSpeed() const
{
	return _speed;
}

void Camera::setSensitivity(float sensitivity)
{
	_sensitivity = sensitivity;
}

const float Camera::getSensitivity() const
{
	return _sensitivity;
}
#pragma endregion
