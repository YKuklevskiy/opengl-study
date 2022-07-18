#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using std::cout;

const int windowHeight = 600;
const int windowWidth = 800;

void handleInput(GLFWwindow* window)
{
	
}


int main()
{
	if (glfwInit() == GLFW_FALSE)
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the window and make it's context current, load glad
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGLStudy", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create a window. Terminating...\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	gladLoadGL();

	while (!glfwWindowShouldClose(window)) 
	{
		handleInput(window);

		glClearColor(0.09f, 0.09f, 0.09f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);



		glfwSwapBuffers(window);
		glfwPollEvents(); 
	}
	
	glfwTerminate();
	return 0;
}