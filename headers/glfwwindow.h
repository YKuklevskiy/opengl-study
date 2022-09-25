#pragma once

#include <gladLoader.h>

#include <GLObjects/GLObjects.h>
#include <GLFW/glfw3.h>
#include <glfwmouseinfo.h>
#include <OpenGLView.h>

class Window
{
public:
	Window(int windowWidth, int windowHeight)
		: windowWidth(windowWidth), windowHeight(windowHeight)
	{
		if (!isGlfwInitialised())
			return;
		setContextHints();
		createWindow();
		makeContextCurrent();
		GladLoader::LoadGlad();
		initView();
	}

	inline void ResizeFramebufferCallback(GLFWwindow* window, int width, int height)
	{
		setWindowSize(width, height);
		setViewportSize(width, height);
	}

	inline void MouseMoveCallback(GLFWwindow* window, int newXPosition, int newYPosition)
	{
		updateFrameMouseOffset(newXPosition, newYPosition);
	}

	inline bool CheckIfWindowShouldClose()
	{
		return glfwWindowShouldClose(window);
	}

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

	bool isGlfwInitialised();
	void setContextHints();
	void createWindow();
	void makeContextCurrent();
	void initView();

	void terminateWindow(const std::string&& message);

	void updateMouseValuesAccordingToOffset();
	void resetMouseOffset();
};