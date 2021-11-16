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


// Variables
GLFWwindow* window;
const int width = 1024, height = 1024;

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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

	Sphere sphere;

	// Bind vao
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Bind vbo
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sphere.getInterleavedVertexSize(), sphere.getInterleavedVertices(), GL_STATIC_DRAW);

	// Bind ibo
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.getIndexSize(), sphere.getIndices(), GL_STATIC_DRAW);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Set attribute pointers
	int stride = sphere.getInterleavedStride();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

	// Check if the window was closed
	while (!glfwWindowShouldClose(window))
	{
		// Terminate window
		processInput(window);

		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use shaders
		glUseProgram(programID);

		glBindVertexArray(vao);

		glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, (void*)0);
	}

	// Cleanup
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vao);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}