// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include "Dependencies\glew\glew.h"

// Include GLFW
#include "Dependencies\glfw\glfw3.h"

// Include GLM
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

#include "shader.hpp"
#include "sphere.hpp"
#include "cylinder.hpp"
#include "camera.hpp"

// Variables
GLFWwindow* window;
const int width = 1024, height = 1024;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
bool isMoving = false;
float ballMovingValue = 0.0f;
float ballRotatingValue = 0.0f;
float cylinderRadius = 0.15f;
float cylinderHeight = 1.0f;
float sphereRadius = 0.3f;

glm::vec3 cameraPos = glm::vec3(300.0f, 0.0f, 100.0f);
//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 500.0f);
glm::vec3 cameraDir = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(100.0f, 0.0f, 0.0f);

Camera camera(cameraPos, cameraDir, cameraUp);

void processInput(GLFWwindow* window)
{
	float cameraSpeed = 1.0f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_Q)) {
		camera.translateUp(cameraSpeed * 10.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_W)) {
		camera.translateDown(cameraSpeed * 10.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_E)) {
		camera.translateFront(cameraSpeed * 100.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_R)) {
		camera.translateBack(cameraSpeed * 100.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		isMoving = true;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		isMoving = false;
		ballMovingValue = 0.0f;
	}
}

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "Bowling Game", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	// Specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Background color
	glClearColor(0.898, 0.627, 0.424, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	GLuint sphereProgramID = LoadShaders("SphereVertexShader.vertexshader", "SphereFragmentShader.fragmentshader");
	GLuint cylinderProgramID = LoadShaders("CylinderVertexShader.vertexshader", "CylinderFragmentShader.fragmentshader");

	Sphere sphere;
	sphere.setRadius(sphereRadius);
	sphere.setSectorCount(72);
	sphere.setStackCount(24);
	sphere.setSmooth(true);

	Cylinder cylinder;
	cylinder.setBaseRadius(cylinderRadius);
	cylinder.setTopRadius(cylinderRadius);
	cylinder.setHeight(cylinderHeight);
	cylinder.setSectorCount(36);
	cylinder.setStackCount(8);
	cylinder.setSmooth(true);

	GLuint sphereVAO, sphereVBO, sphereIBO, cylinderVAO, cylinderVBO, cylinderIBO;

	// ------------------------------ Sphere ------------------------------
	// Bind vao
	glGenVertexArrays(1, &sphereVAO);
	glBindVertexArray(sphereVAO);
	// Bind vbo
	glGenBuffers(1, &sphereVBO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sphere.getInterleavedVertexSize(), sphere.getInterleavedVertices(), GL_STATIC_DRAW);
	// Bind ibo
	glGenBuffers(1, &sphereIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.getIndexSize(), sphere.getIndices(), GL_STATIC_DRAW);
	// Activate attribute arrays
	glEnableVertexAttribArray(0);
	// Set attribute pointers
	int sphereStride = sphere.getInterleavedStride();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sphereStride, (void*)0);

	// ------------------------------ Cylinder ------------------------------
	// Bind vao
	glGenVertexArrays(1, &cylinderVAO);
	glBindVertexArray(cylinderVAO);
	// Bind vbo
	glGenBuffers(1, &cylinderVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
	glBufferData(GL_ARRAY_BUFFER, cylinder.getInterleavedVertexSize(), cylinder.getInterleavedVertices(), GL_STATIC_DRAW);
	// Bind ibo
	glGenBuffers(1, &cylinderIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinder.getIndexSize(), cylinder.getIndices(), GL_STATIC_DRAW);
	// Activate attribute arrays
	glEnableVertexAttribArray(0);
	// Set attribute pointers
	int cylinderStride = cylinder.getInterleavedStride();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, cylinderStride, (void*)0);

	// Camera projection
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);

	// Wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Check if the window was closed
	while (!glfwWindowShouldClose(window))
	{
		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();
		processInput(window);

		// Depth buffer enabled
		glEnable(GL_DEPTH_TEST);

		// Color buffer enabled
		glEnable(GL_COLOR_MATERIAL);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use shaders
		glUseProgram(programID);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(camera.getCameraPosition(), glm::vec3(0, 0, 0), camera.getCameraUp());

		bool isColloid = ballMovingValue >= (3.0f - (cylinderRadius + sphereRadius));

		// ------------------------------ Cylinder MVP ------------------------------

		// Cylinder shader
		glUseProgram(cylinderProgramID);

		glm::mat4 cylindersModel = glm::mat4(1.0f);
		float cylinderAngle = 0.0f;
		if (isColloid)
		{
			cylinderAngle = 90.0f;
		}

		cylindersModel = glm::translate(cylindersModel, glm::vec3(3.0f, 0.0f, -(cylinderHeight / 2)));
		cylindersModel = glm::rotate(cylindersModel, cylinderAngle, glm::vec3(0.0, -(cylinderHeight / 2), 0.0));

		glm::mat4 mvp = projection * view * cylindersModel;
		unsigned int mvpLoc = glGetUniformLocation(cylinderProgramID, "mvp");
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

		glBindVertexArray(cylinderVAO);
		glDrawElements(GL_TRIANGLES, cylinder.getIndexCount(), GL_UNSIGNED_INT, (void*)0);

		// ------------------------------ Sphere MVP ------------------------------

		// Sphere shader
		glUseProgram(sphereProgramID);
		glm::mat4 sphereModel = glm::mat4(1.0f);

		/*
		Ball launched:
		To get the first contact, radius of the sphere and the cylinder should be removed from the translation value of the cylinder
		*/
		if (isMoving && !isColloid)
		{
			ballMovingValue += 0.001f;
			ballRotatingValue += 1.0f;
		}

		sphereModel = glm::translate(sphereModel, glm::vec3(ballMovingValue, 0.0f, -sphereRadius));
		sphereModel = glm::rotate(sphereModel, ballRotatingValue, glm::vec3(0.0, 0.0, 1.0));

		glm::mat4 mvp2 = projection * view * sphereModel;
		unsigned int mvp2Loc = glGetUniformLocation(sphereProgramID, "mvp");
		glUniformMatrix4fv(mvp2Loc, 1, GL_FALSE, glm::value_ptr(mvp2));

		glBindVertexArray(sphereVAO);
		glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, (void*)0);
	}

	// Cleanup
	glDeleteBuffers(1, &sphereVBO);
	glDeleteBuffers(1, &sphereIBO);
	glDeleteBuffers(1, &sphereVAO);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}