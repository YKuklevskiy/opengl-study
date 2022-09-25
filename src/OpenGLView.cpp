#include <OpenGLView.h>
#include <GLFW/glfw3.h>
#include <constants.h>
#include <glfwwindow.h>

inline void OpenGLView::SetCulling(bool isCullingEnabled)
{
	if(isCullingEnabled)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

inline void OpenGLView::SetDepthBuffer(bool isDepthBufferEnabled)
{
	if(isDepthBufferEnabled)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

const std::shared_ptr<Camera> OpenGLView::GetBoundCamera() const
{
	return boundCamera;
}

void OpenGLView::setDefaultParameters()
{
	SetCulling(true);
	SetDepthBuffer(true);
}

void OpenGLView::initDefaultCamera()
{
	initCamera(INITIAL_CAMERA_POSITION, INITIAL_YAW, INITIAL_PITCH, SENSITIVITY, CAMERA_SPEED);
}

void OpenGLView::initCamera(const glm::vec3& position, float yaw, float pitch, float sensitivity, float flySpeed)
{
	boundCamera = std::make_shared<Camera>(INITIAL_CAMERA_POSITION, INITIAL_YAW, INITIAL_PITCH);
	boundCamera->setSensitivity(SENSITIVITY);
	boundCamera->setSpeed(CAMERA_SPEED);
}

void OpenGLView::handleInput(const MouseInfo& mouse, const float deltaTime)
{
	handlePolygonRenderMode();
	updateMovementEnabled();
	updateCursorVisibility();
	if (isMovementEnabled)
		handleMovement(mouse, deltaTime);
}

void OpenGLView::handlePolygonRenderMode()
{
	if (isButtonPressed(GLFW_KEY_1))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OpenGLView::updateMovementEnabled()
{
	if (isButtonPressed(GLFW_KEY_LEFT_SHIFT))
		enableMovement(true);
	else
		enableMovement(false);
}

void OpenGLView::enableMovement(bool setTo)
{
	isMovementEnabled = (setTo == true);
}

void OpenGLView::updateCursorVisibility()
{
	if (isMovementEnabled)
		enableCursor(false);
	else
		enableCursor(true);
}

void OpenGLView::enableCursor(bool setTo)
{
	if (setTo == true)
		setCursorMode(GLFW_CURSOR_NORMAL);
	else
		setCursorMode(GLFW_CURSOR_DISABLED);
}

void OpenGLView::setCursorMode(int modeKey)
{
	glfwSetInputMode(getGLFWwindowInstance(), GLFW_CURSOR, modeKey);
}

void OpenGLView::handleMovement(const MouseInfo& mouse, const float deltaTime)
{
	handleCameraRotation(mouse);
	handleCameraTranslation(deltaTime);
}

void OpenGLView::handleCameraRotation(const MouseInfo& mouse)
{
	boundCamera->handleRotation(mouse);
}

void OpenGLView::handleCameraTranslation(const float deltaTime)
{
	auto velocity = getVelocityDirection() * deltaTime;
	translateCamera(velocity);
}

glm::vec3 OpenGLView::getVelocityDirection()
{
	glm::vec3 dir = glm::vec3(0.0f, 0.0f, 0.0f);

	if (isButtonPressed(GLFW_KEY_D))
		dir.x += 1;
	if (isButtonPressed(GLFW_KEY_A))
		dir.x -= 1;

	if (isButtonPressed(GLFW_KEY_SPACE))
		dir.y += 1;
	if (isButtonPressed(GLFW_KEY_C))
		dir.y -= 1;

	if (isButtonPressed(GLFW_KEY_W))
		dir.z += 1;
	if (isButtonPressed(GLFW_KEY_S))
		dir.z -= 1;

	return dir;
}

void OpenGLView::translateCamera(const glm::vec3& velocity)
{
	boundCamera->handleMovement(velocity);
}

bool OpenGLView::isButtonPressed(int GLFWkeyCode)
{
	return parentWindow.lock()->IsButtonPressed(GLFWkeyCode);
}

GLFWwindow* OpenGLView::getGLFWwindowInstance()
{
	return parentWindow.lock()->GetGLFWwindowInstance();
}