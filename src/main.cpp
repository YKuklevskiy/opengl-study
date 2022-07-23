#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include "shader.h"
#include "VBO.h"
#include "VAO.h"

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

	//
	//		Load textures
	//

	int textureWidth, textureHeight, textureChannelsCount;
	GLubyte* textureData = stbi_load("textures/doom_brick.png",
		&textureWidth, &textureHeight, &textureChannelsCount, 4);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	if (textureData) // generate texture
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);

		cout << "SUCCESS: TEXTURE LOADED\n";
	}
	else
		cout << "ERROR: COULD NOT READ TEXTURE FILE\n";

	stbi_image_free(textureData);

	//
	//		Setup data buffer
	//

	GLfloat triangleVertexArray[] =
	{
//	    __position__    texturecoords
		-0.5f, -0.5f,     0.0f,  0.0f,
		 0.0f,  0.5f,     0.5f,  1.0f,
		 0.5f, -0.5f,     1.0f,  0.0f, 
	};

	// Create and bind VAO (which will store VBO and attributes)
	VAO vao;
	vao.bind();

	// Create and bind VBO
	VBO vbo;
	vbo.bufferData(triangleVertexArray);

	// Setup vertex attributes
	VertexAttribute positionAttribute = { 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0 };
	vao.setupVertexAttribute(positionAttribute);

	VertexAttribute textureCoordinateAttribute = { 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)) };
	vao.setupVertexAttribute(textureCoordinateAttribute);

	//
	//		Frame Loop
	//

	while (!glfwWindowShouldClose(window)) 
	{
		handleInput(window);

		glClearColor(0.09f, 0.09f, 0.09f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		vao.bind();
		glBindTexture(GL_TEXTURE_2D, texture);
		shaderProgram.use();
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents(); 
	}
	
	glfwTerminate();
	return 0;
}