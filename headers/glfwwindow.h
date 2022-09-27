#pragma once

#include <gladLoader.h>

#include <GLObjects/GLObjects.h>
#include <GLFW/glfw3.h>
#include <glfwmouseinfo.h>

class OpenGLView;


/// <summary>
/// 
/// GLFW Window wrapper class. Does all the initialization stuff, and holds the OpenGLView (custom class) 
/// bound to this window. 
/// 
/// Constructor initializes the window. View initialization is done by calling InitializeView();
/// 
/// </summary>
class Window : public std::enable_shared_from_this<Window>
{
public:
	inline Window(int windowWidth, int windowHeight)
		: windowWidth(windowWidth), windowHeight(windowHeight)
	{
		initializeWindow();
	}

	void InitializeView();

	void ResizeFramebufferCallback(GLFWwindow* window, int width, int height);
	void MouseMoveCallback(GLFWwindow* window, int newXPosition, int newYPosition);

	bool CheckIfWindowShouldClose() const;

	void Update(float deltaTime);

	void SwapBuffers();
	void PollGLEvents();

	GLFWwindow* GetGLFWwindowInstance() const;
	bool IsMinimized() const;
	bool IsButtonPressed(int GLFWkeyCode) const;
	float GetAspectRatio() const;
	const std::shared_ptr<Camera> GetBoundCamera() const;

private:
	GLFWwindow* window;
	MouseInfo mouse;
	int windowWidth;
	int windowHeight;

	std::shared_ptr<OpenGLView> boundView;

	void setWindowSize(int width, int height);
	void setViewportSize(int width, int height);

	void updateFrameMouseOffset(int newXPosition, int newYPosition);

	void initializeWindow();
	bool isGlfwInitialised();
	void setContextHints();
	void createWindow();
	void makeContextCurrent();

	void terminateWindow(const std::string&& message);

	void updateMouseValuesAccordingToOffset();
	void resetMouseOffset();

	std::shared_ptr<Window> getSharedPointerFromThis();
};