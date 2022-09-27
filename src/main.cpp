#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <glfwwindow.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLObjects/GLObjects.h>

#include <constants.h>
#include <renderer.h>
#include <material.h>
#include <model.h>

/*

Need to polish window and view classes, and abstract mouse work into MouseInfo (change name?) class

*/

using std::cout;
using std::string;

std::shared_ptr<Window> win_Window;

// from https://www.khronos.org/opengl/wiki/Example/OpenGL_Error_Testing_with_Message_Callbacks
void GLAPIENTRY
MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

#pragma region CallbackBinds

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	win_Window->ResizeFramebufferCallback(window, width, height);
}

void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	win_Window->MouseMoveCallback(window, xpos, ypos);
}

#pragma endregion

int main()
{
	win_Window = std::make_shared<Window>(WINDOW_WIDTH, WINDOW_HEIGHT);
	win_Window->InitializeView();

	// bind callback functions
	glfwSetFramebufferSizeCallback(win_Window->GetGLFWwindowInstance(), framebufferSizeCallback);
	glfwSetCursorPosCallback(win_Window->GetGLFWwindowInstance(), mouseMoveCallback);

	// GL error logging setup
#ifdef DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	// disable gl notifications
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE); 
#endif 

	//
	//		SHADERS
	//

	Shader lightShader("lightVertexShader.glsl", "lightFragmentShader.glsl");
	if (!lightShader.isValid())
	{
		cout << "Failed to compile and link shaders.";
	}

	Shader modelShader("modelVertexShader.glsl", "modelFragmentShader.glsl");
	if (!modelShader.isValid())
	{
		cout << "Failed to compile and link shaders.";
	}
	modelShader.use();
	modelShader.setVec3f("light.ambient", AMBIENT_LIGHT_COLOR);
	modelShader.setVec3f("light.diffuse", DIFFUSE_LIGHT_COLOR);
	modelShader.setVec3f("light.specular", SPECULAR_LIGHT_COLOR);

	//
	//		Setup light cube data buffer and load models
	//

	std::vector<GLfloat> cubeVertexArray =
	{
		// position
		-0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};

	Model backpackModel("models/backpack/backpack.obj");

	// Create and bind VAO (which will store VBO and attributes)
	VAO vao;
	vao.bind();

	// Create and bind VBO
	VBO vbo;
	vbo.bind();
	vbo.bufferData(cubeVertexArray);

	// Setup vertex attributes
	VertexAttributeLayout layout;
	layout.AddAttribute<GLfloat>(3); // position
	vao.setupVertexAttributes(layout);

	//
	//		Frame Loop
	//

	vao.bind();

	Renderer renderer;
	renderer.setClearColor(0.09f, 0.09f, 0.09f);

	double curTime = glfwGetTime();

	while (!win_Window->CheckIfWindowShouldClose())
	{
		renderer.clear();

		double prevTime = curTime;
		curTime = glfwGetTime();
		float deltaTime = curTime - prevTime;

		win_Window->Update(deltaTime);

		if (win_Window->IsMinimized())
		{
			win_Window->SwapBuffers();
			win_Window->PollGLEvents();
			continue;
		}

		float time = sin(curTime * TIME_SPEED_MODIFIER);

		// setup lightsource cube
		lightShader.use();

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::rotate(modelMatrix, (float)curTime * 0.4f, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
		modelMatrix = glm::rotate(modelMatrix, (float)sin(curTime * 0.25f) * 0.33f, glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
		modelMatrix = glm::translate(modelMatrix, INITIAL_LIGHT_POSITION);
		modelMatrix = glm::scale(modelMatrix, { 0.5f, 0.5f, 0.5f });

		std::shared_ptr<Camera> boundCamera = win_Window->GetBoundCamera();

		glm::mat4 viewMatrix = glm::mat4(1.0f);
		glm::vec3 cameraPosition = boundCamera->getPosition();
		glm::vec3 cameraDirection = boundCamera->getDirectionVector();
		viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, glm::vec3(0, 1, 0));

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(FOV), win_Window->GetAspectRatio(), 0.1f, 100.0f);

		glm::vec3 lightPosition = viewMatrix * modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // view space calc

		lightShader.setMat4f("model", modelMatrix);
		lightShader.setMat4f("view", viewMatrix);
		lightShader.setMat4f("projection", projectionMatrix);

		//render lightsource cube
		renderer.drawVertices(lightShader, vbo, vao);

		// setup backpack
		modelShader.use();

		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::rotate(modelMatrix, time, glm::normalize(glm::vec3(0.0f, 0.5f, 1.0f)));

		// NOTE: calculations are done in view space

		glm::mat3 normalMatrix = glm::mat3(viewMatrix * modelMatrix); 
		normalMatrix = glm::transpose(glm::inverse(normalMatrix));

		modelShader.setMat4f("model", modelMatrix);
		modelShader.setMat4f("view", viewMatrix);
		modelShader.setMat4f("projection", projectionMatrix);
		modelShader.setMat3f("normalMatrix", normalMatrix);

		modelShader.setVec3f("light.position", lightPosition);

		//render backpack
		backpackModel.Draw(modelShader);

		win_Window->SwapBuffers();
		win_Window->PollGLEvents();
	}

	glfwTerminate();
	return 0;
}