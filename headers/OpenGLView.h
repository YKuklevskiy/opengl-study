#pragma once

#include <memory>
#include <GLObjects/camera.h>
#include <glfwmouseinfo.h>

struct GLFWwindow;

class OpenGLView
{
public:
	inline OpenGLView(std::weak_ptr<class Window> window)
		: parentWindow(window)
	{
		setDefaultParameters();
		initDefaultCamera();
	}

	void SetCulling(bool isCullingEnabled);
	void SetDepthBuffer(bool isDepthBufferEnabled);

	void UpdateView(const MouseInfo& mouse, const float deltaTime)
	{
		handleInput(mouse, deltaTime);
	}

	const std::shared_ptr<Camera> GetBoundCamera() const;

private:
	std::shared_ptr<Camera> boundCamera;
	std::weak_ptr<Window> parentWindow;

	bool isMovementEnabled = false;

	void setDefaultParameters();


	void initDefaultCamera();
	void initCamera(const glm::vec3& position, float yaw, float pitch, float sensitivity, float flySpeed);


	void handleInput(const MouseInfo& mouse, const float deltaTime);

	void handlePolygonRenderMode();

	void updateMovementEnabled();
	void enableMovement(bool setTo);

	void updateCursorVisibility();
	void enableCursor(bool setTo);
	void setCursorMode(int modeKey);

	void handleMovement(const MouseInfo& mouse, const float deltaTime);
	void handleCameraRotation(const MouseInfo& mouse);
	void handleCameraTranslation(const float deltaTime);
	glm::vec3 getVelocityDirection();
	void translateCamera(const glm::vec3& velocity);

	bool isButtonPressed(int GLFWkeyCode);
	GLFWwindow* getGLFWwindowInstance();
};