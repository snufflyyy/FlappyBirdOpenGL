#include "render.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "window.h"
#include "sprite.h"

// said this in window.c but
// not a fan of globals but its
// fine for this very simple game
unsigned int VAO, VBO, EBO;
mat4 projection;

static double previousTime;
static int frameCount;

void initRenderer() {
	// create vertex array object
	glGenVertexArrays(1, &VAO);

	// create vertex buffer object
	glGenBuffers(1, &VBO);

	// create element buffer object
	glGenBuffers(1, &EBO);

	// init projection
	glm_mat4_identity(projection);
	glm_ortho(0, windowWidth, 0, windowHeight, -1, 1, projection);

	previousTime = glfwGetTime();
	frameCount = 0;
}

void beginRendering() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void clearBackground(float r, float g, float b) {
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void printFPS() {
	double currentTime = glfwGetTime();
	frameCount++;

	if (currentTime - previousTime >= 1.0) {
		printf("fps: %d\n", frameCount);

		frameCount = 0;
		previousTime = currentTime;
	}
}

void endRendering() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void deinitRenderer() {
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}