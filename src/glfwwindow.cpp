#include <glfwwindow.h>
#include <memory>
#include <OpenGLView.h>

void Window::Update(float deltaTime)
{
	updateMouseValuesAccordingToOffset(); // because we want to work with new mouse position for the new frame
	boundView->UpdateView(mouse, deltaTime);
	resetMouseOffset();
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void Window::PollGLEvents()
{
	glfwPollEvents();
}

GLFWwindow* Window::GetGLFWwindowInstance() const
{
	return window;
}

bool Window::IsMinimized() const
{
	return (windowWidth == 0 || windowHeight == 0);
}

bool Window::IsButtonPressed(int GLFWkeyCode) const
{
	return glfwGetKey(window, GLFWkeyCode) == GLFW_PRESS;
}

float Window::GetAspectRatio() const
{
	return (float)windowWidth / windowHeight;
}

const std::shared_ptr<Camera> Window::GetBoundCamera() const
{
	return boundView->GetBoundCamera();
}

void Window::setWindowSize(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
}

void Window::setViewportSize(int width, int height)
{
	glViewport(0, 0, windowWidth, windowHeight);
}

void Window::updateFrameMouseOffset(int newXPosition, int newYPosition)
{
	mouse.frameMouseOffsetX = newXPosition - mouse.previousFrameMouseX;

	// inverted since y-coordinates range from bottom to top
	mouse.frameMouseOffsetY = -(newYPosition - mouse.previousFrameMouseY);
}

bool Window::isGlfwInitialised()
{
	return glfwInit() == GLFW_TRUE;
}

void Window::setContextHints()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Window::createWindow()
{
	std::string windowName = "OpenGLStudy";
	window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), NULL, NULL);
	if (window == NULL)
		terminateWindow("Failed to create a window.");
}

void Window::makeContextCurrent()
{
	glfwMakeContextCurrent(window);
}

void Window::initView()
{
	boundView = std::make_shared<OpenGLView>();
	boundView->SetParentWindow(getSharedPointerFromThis());
}

void Window::terminateWindow(const std::string&& message)
{
	cout << message + " Terminating...\n";
	glfwTerminate();
}

void Window::updateMouseValuesAccordingToOffset()
{
	mouse.previousFrameMouseX += mouse.frameMouseOffsetX;
	mouse.previousFrameMouseY += mouse.frameMouseOffsetY;
}

void Window::resetMouseOffset()
{
	mouse.frameMouseOffsetX = 0;
	mouse.frameMouseOffsetY = 0;
}

std::shared_ptr<Window> Window::getSharedPointerFromThis()
{
	return shared_from_this();
}
