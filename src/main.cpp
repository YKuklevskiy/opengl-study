#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <constants.h>
#include <GLObjects/shader.h>
#include <GLObjects/VBO.h>
#include <GLObjects/VAO.h>
#include <GLObjects/EBO.h>
#include <GLObjects/texture.h>
#include <GLObjects/camera.h>
#include <Renderer.h>

using std::cout;
using std::string;

int windowHeight = 600;
int windowWidth = 800;
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

	//
	//		Load shaders
	//

	Shader objectShader("vertexShader.glsl", "fragmentShader.glsl");
	if (!objectShader.isValid())
	{
		cout << "Failed to compile and link shaders. Terminating...\n";
		glfwTerminate();
		return -1;
	}
	objectShader.use();

	Shader lightShader("lightVertexShader.glsl", "lightFragmentShader.glsl");
	if (!lightShader.isValid())
	{
		cout << "Failed to compile and link shaders. Terminating...\n";
		glfwTerminate();
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	//
	//		Setup camera
	//

	boundCamera = new Camera(INITIAL_POSITION, INITIAL_YAW, INITIAL_PITCH);
	boundCamera->setSensitivity(SENSITIVITY);
	boundCamera->setSpeed(CAMERA_SPEED);

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

	/////// TODO Too much checking for validity, need to encapsulate all this setup stuff into Window class ///////

	//
	//		Setup data buffer
	//

	GLfloat cubeVertexArray[] =
	{
		// position			  texturecoords       normals
		-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   0.0f, 0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   0.0f, 0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f, 0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f, 0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   0.0f, 0.0f, -1.0f,
							   			     
		-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
							   			     
		-0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
							   			     
		 0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
							   			     
		-0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,   1.0f, 1.0f,   0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   0.0f, -1.0f, 0.0f,
							   			     
		-0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f
	};

	/*GLuint indices[] =
	{
		0, 1, 2
	};*/

	// Create and bind VAO (which will store VBO and attributes)
	VAO vao;
	vao.bind();

	// Create and bind VBO
	VBO vbo;
	vbo.bind();
	vbo.bufferData(cubeVertexArray);

	/*EBO ebo;
	ebo.bind();
	ebo.bufferData(indices);*/

	// Setup vertex attributes
	VertexAttributeLayout layout;
	layout.AddAttribute<GLfloat>(3); // position
	layout.AddAttribute<GLfloat>(2); // texCoords
	layout.AddAttribute<GLfloat>(3); // normals
	vao.setupVertexAttributes(layout);

	//
	//		Frame Loop
	//

	vao.bind();
	objectShader.use();
	objectShader.setInt("_texture", 0);
	objectShader.setInt("_normalTexture", 1);
	objectShader.setFloat("ambientLight", AMBIENT_LIGHT);

	Renderer renderer;
	renderer.setClearColor(0.09f, 0.09f, 0.09f);

	double curTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		double prevTime = curTime;
		curTime = glfwGetTime();
		float deltaTime = curTime - prevTime;

		handleInput(window, deltaTime);

		renderer.clear();

		const float speedModifier = 2.0f;
		float time = sin(curTime * speedModifier);
		objectShader.use();
		objectShader.setFloat("time", time * 0.5f + 0.5f);


		// setup cube
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::rotate(modelMatrix, time, glm::normalize(glm::vec3(0.0f, 0.5f, 1.0f)));

		glm::mat4 viewMatrix = glm::mat4(1.0f);
		glm::vec3 cameraPosition = boundCamera->getPosition();
		glm::vec3 cameraDirection = boundCamera->getDirectionVector();
		viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, glm::vec3(0, 1, 0));

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(FOV), (float)windowWidth / windowHeight, 0.1f, 100.0f);

		objectShader.setMat4f("model", modelMatrix);
		objectShader.setMat4f("view", viewMatrix);
		objectShader.setMat4f("projection", projectionMatrix);

		// setup lightsource cube
		lightShader.use();
		
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::rotate(modelMatrix, (float)curTime, glm::normalize(glm::vec3(1.0f, 0.25f, 0.5f)));
		modelMatrix = glm::rotate(modelMatrix, (float)curTime * 0.33f, glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));
		modelMatrix = glm::translate(modelMatrix, { 0, 2, 0 });
		modelMatrix = glm::scale(modelMatrix, { 0.5f, 0.5f, 0.5f });

		lightShader.setMat4f("model", modelMatrix);
		lightShader.setMat4f("view", viewMatrix);
		lightShader.setMat4f("projection", projectionMatrix);

		glm::vec3 lightPosition = modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		objectShader.use();
		objectShader.setVec3f("lightPosition", lightPosition);

		// render cube
		renderer.drawVertices(objectShader, vbo, vao);
		//render lightsource cube
		renderer.drawVertices(lightShader, vbo, vao);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}