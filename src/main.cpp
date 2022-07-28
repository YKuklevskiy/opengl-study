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
#include "camera.h"

using std::cout;
using std::string;

int windowHeight = 600;
int windowWidth = 800;
const float FOV = 75.0f;
const float initialYaw = -90.0f;
const float initialPitch = 0.0f;
const float sensitivity = 0.25f;
const float cameraSpeed = 4.0f;
const glm::vec3 initialPosition(0.0f, 0.0f, 2.0f);
Camera* boundCamera = nullptr;

void handleInput(GLFWwindow* window, float deltaTime)
{
	//
	// wireframe view
	//

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	//
	// camera lookaround
	//

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		boundCamera->enableMovement();
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		boundCamera->disableMovement();
	}

	//
	// moving around the scene
	//

	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		velocity.z += 1;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		velocity.z -= 1;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		velocity.x -= 1;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		velocity.x += 1;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		velocity.y += 1;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		velocity.y -= 1;
	}

	boundCamera->handleMovement(velocity * deltaTime);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;

	glViewport(0, 0, windowWidth, windowHeight);
}

float lastMouseX = windowWidth / 2, lastMouseY = windowHeight / 2;
void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = (xpos - lastMouseX);
	float yoffset = -(ypos - lastMouseY); // reversed since y-coordinates range from bottom to top

	boundCamera->handleRotation(xoffset, yoffset);

	lastMouseX = xpos;
	lastMouseY = ypos;
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

	// enabling raw mouse input
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	// binding callback functions
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseMoveCallback);

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
	glEnable(GL_DEPTH_TEST);

	boundCamera = new Camera(initialPosition, initialYaw, initialPitch);
	boundCamera->setSensitivity(sensitivity);
	boundCamera->setSpeed(cameraSpeed);

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

	glm::vec3 triangleScenePositions[] =
	{
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(0, 0, -1)
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

	double curTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		double prevTime = curTime;
		curTime = glfwGetTime();
		float deltaTime = curTime - prevTime;

		handleInput(window, deltaTime);

		glClearColor(0.09f, 0.09f, 0.09f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const float speedModifier = 2.0f;
		float time = sin(curTime * speedModifier);

		for (int i = 0; i < sizeof(triangleScenePositions) / sizeof(glm::vec3); i++)
		{
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, triangleScenePositions[i]);
			modelMatrix = glm::rotate(modelMatrix, time, glm::normalize(glm::vec3(0.0f, 0.5f, 1.0f)));

			glm::mat4 viewMatrix = glm::mat4(1.0f);
			glm::vec3 cameraPosition = boundCamera->getPosition();
			glm::vec3 cameraDirection = boundCamera->getDirectionVector();
			viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, glm::vec3(0, 1, 0));

			glm::mat4 projectionMatrix = glm::perspective(glm::radians(FOV), (float)windowWidth / windowHeight, 0.1f, 100.0f);

			shaderProgram.setFloat("time", time * 0.5f + 0.5f);
			shaderProgram.setMat4f("model", modelMatrix);
			shaderProgram.setMat4f("view", viewMatrix);
			shaderProgram.setMat4f("projection", projectionMatrix);

			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}