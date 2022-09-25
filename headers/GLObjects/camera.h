#pragma once

#include "glm/glm.hpp"
#include <glfwmouseinfo.h>

class Camera
{
public:
	Camera(glm::vec3 initialPosition, float initialYaw, float initialPitch);

	void handleMovement(glm::vec3 offset, double speedModifier = 1.0f);
	void handleRotation(const MouseInfo& mouse, double sensitivityModifier = 1.0f);

	const glm::vec3& getPosition() const;
	const glm::vec3 getDirectionVector() const;

	const float getYaw() const;

	const float getPitch() const;

	void setSpeed(float speed);
	const float getSpeed() const;

	void setSensitivity(float sensitivity);
	const float getSensitivity() const;

private:
	glm::vec3 _position;
	float _yaw;
	float _pitch;
	float _speed = 1.0f;
	float _sensitivity = 1.0f;
};