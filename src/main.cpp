#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>

#include "shader.h"

using std::cout;
using std::string;

const int windowHeight = 600;
const int windowWidth = 800;

const string SHADER_FOLDER = "src/shaders/";

void handleInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void createAndBindVAO(GLuint& VAO)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
}

template <size_t n>
void createAndBindVBO(GLuint& VBO, GLfloat (&vertexArray)[n])
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
}

struct VertexAttribute
{
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	GLvoid* offset;
};

void setupVertexAttribute(VertexAttribute attribute)
{
	glEnableVertexAttribArray(attribute.index);
	glVertexAttribPointer(
		attribute.index, 
		attribute.size, 
		attribute.type, 
		attribute.normalized, 
		attribute.stride, 
		attribute.offset
	);
}

int main()
{
	if (glfwInit() == GLFW_FALSE)
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//
	//		Create the window and make it's context current, load glad
	//

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGLStudy", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create a window. Terminating...\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD. Terminating...\n";
		glfwTerminate();
		return -1;
	}
	gladLoadGL();

	Shader shaderProgram("vertexShader.glsl", "fragmentShader.glsl");
	if (!shaderProgram.isValid())
	{
		cout << "Failed to compile and link shaders. Terminating...\n";
		glfwTerminate();
		return -1;
	}
	shaderProgram.use();

	GLfloat triangleVertexArray[] =
	{
//	    __position__  _____color______
		-0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 
		 0.0f,  0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f
	};

	//
	//		Setup data buffer
	//

	// Create and bind VAO (which will store VBO and attributes)
	GLuint VAO;
	createAndBindVAO(VAO);

	// Create and bind VBO
	GLuint VBO;
	createAndBindVBO(VBO, triangleVertexArray);

	// Setup vertex attributes
	VertexAttribute positionAttribute = { 0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0 };
	setupVertexAttribute(positionAttribute);

	VertexAttribute colorAttribute = { 1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)) };
	setupVertexAttribute(colorAttribute);

	//
	//		Frame Loop
	//

	while (!glfwWindowShouldClose(window)) 
	{
		handleInput(window);

		glClearColor(0.09f, 0.09f, 0.09f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		shaderProgram.use();
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
		glfwPollEvents(); 
	}
	
	glfwTerminate();
	return 0;
}