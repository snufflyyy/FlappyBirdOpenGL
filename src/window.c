#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "render.h"

// not a big fan of globals but i think
// for this small game it is okay
int windowWidth;
int windowHeight;

GLFWwindow* window;

void windowResizeCallback(GLFWwindow* window, int width, int height);

void createWindow(int width, int height, const char *title) {
	// init glfw
	glfwInit();

	// tell glfw that the window will NOT be resized
	glfwWindowHint(GLFW_RESIZABLE, false);

	// tell glfw what opengl version and profile we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// macOS only!
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// create the window
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window) {
		printf("Failed to create Window!\n");
		glfwTerminate();
		exit(-1);
	}

	// set window width and height
	windowWidth = width;
	windowHeight = height;

	// set window as current context
	glfwMakeContextCurrent(window);

	glfwSwapInterval(0);

	// load glad
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		printf("Failed to Load GLAD!\n");
		glfwTerminate();
		exit(-1);
	}

	// set opengl viewport
	glViewport(0, 0, windowWidth, windowHeight);

	// set the function that is run when the window is resized
	glfwSetFramebufferSizeCallback(window, windowResizeCallback);
}

// gets called when window is resized (unused)
void windowResizeCallback(GLFWwindow* window, int width, int height) {
	// updates opengl viewport to new window size
	glViewport(0, 0, windowWidth, windowHeight);

	glm_mat4_identity(projection);
	glm_ortho(0, windowWidth, 0, windowHeight, -1, 1, projection);

	// set window width and height
	windowWidth = width;
	windowHeight = height;
}