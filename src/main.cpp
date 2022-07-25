#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "VBO.h"
#include "VAO.h"
#include "texture.h"

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

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	if (glfwInit() == GLFW_FALSE)
		return -1;

	// Flags and hints
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
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
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

	Texture texture1("brick.jpg", GL_TEXTURE0);
	if (!texture1.isValid())
	{
		cout << "Failed to load texture. Terminating...\n";
		glfwTerminate();
		return -1;
	}
	texture1.setFiltering(GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST);
	Texture texture2("brick_normal.jpg", GL_TEXTURE1);
	if (!texture2.isValid())
	{
		cout << "Failed to load texture. Terminating...\n";
		glfwTerminate();
		return -1;
	}
	texture2.setFiltering(GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST);

	/////// TODO Too much checking for validity, need to implement GL error checking and asserting ///////

	//
	//		Setup data buffer
	//

	GLfloat triangleVertexArray[] =
	{
		// position     texturecoords
		-0.5f, -0.5f,    0.0f,  0.0f,
		 0.0f,  0.5f,    0.5f,  1.0f,
		 0.5f, -0.5f,    1.0f,  0.0f,
	};

	// Create and bind VAO (which will store VBO and attributes)
	VAO vao;
	vao.bind();

	// Create and bind VBO
	VBO vbo;
	vbo.bind();
	vbo.bufferData(triangleVertexArray);

	// Setup vertex attributes
	VertexAttributeLayout layout;
	layout.AddAttribute<GLfloat>(2); // position
	layout.AddAttribute<GLfloat>(2); // texCoords
	vao.setupVertexAttributes(layout);

	//
	//		Frame Loop
	//

	vao.bind();
	shaderProgram.setInt("_texture", 0);
	shaderProgram.setInt("_normalTexture", 1);
	shaderProgram.use();

	while (!glfwWindowShouldClose(window))
	{
		handleInput(window);

		glClearColor(0.09f, 0.09f, 0.09f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		const float speedModifier = 1.0f;
		const float rotationCircleRadius = 0.5f;
		float time = glfwGetTime();
		float phase = sin(time * speedModifier);
		glm::mat4 transformMatrix = glm::mat4(1.0f);

		float translate_x = cos(time) * rotationCircleRadius;
		float translate_y = sin(time) * rotationCircleRadius;

		transformMatrix = glm::translate(transformMatrix, glm::vec3(translate_x, translate_y, 0.0f));
		transformMatrix = glm::rotate(transformMatrix, phase, glm::vec3(0.0f, 0.0f, 1.0f));
		shaderProgram.setFloat("time", phase * 0.5f + 0.5f);
		shaderProgram.setMat4f("transform", transformMatrix);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		// The second triangle will rotate in the opposite direction and pulse with time instead of rotating.

		float scale = phase * 0.5f + 0.5f;

		transformMatrix = glm::mat4(1.0f);
		transformMatrix = glm::translate(transformMatrix, glm::vec3(-translate_x, translate_y, 0.0f));
		transformMatrix = glm::scale(transformMatrix, glm::vec3(scale, scale, scale));
		shaderProgram.setMat4f("transform", transformMatrix);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}