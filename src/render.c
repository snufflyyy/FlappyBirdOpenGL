#include "render.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "window.h"

// for this game i am only rendering quads so this is good!!
const float vertices[] = {
	// position			   // texture coords	
     0.5f,  0.5f,  1.0f, 1.0f,  // top right
     0.5f, -0.5f,  1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
   -0.5f, 0.5f, 0.0f, 1.0f  // top left 
};
const unsigned int indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  

// said this in window.c but
// not a fan of globals but its
// fine for this very simple game
unsigned int VAO, VBO, EBO;
mat4 projection;

int framerate;
float deltaTime;

static float previousTimeFPS;
static float previousTimeDeltaTime;
static int frameCount;

void initRenderer() {
	// create vertex array object
	glGenVertexArrays(1, &VAO);
	// create vertex buffer object
	glGenBuffers(1, &VBO);
	// create element buffer object
	glGenBuffers(1, &EBO);

    // bind the vertices and the indices
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

	// init projection
	glm_mat4_identity(projection);
	glm_ortho(0, windowWidth, 0, windowHeight, -1, 1, projection);

    // used for frame rate
	previousTimeFPS = (float) glfwGetTime();
	frameCount = 0;
}

void beginRendering() {
    // fps and delta time calculation
    float currentTime = (float) glfwGetTime();
    frameCount++;

    deltaTime = currentTime - previousTimeDeltaTime;
    previousTimeDeltaTime = currentTime;

    if (currentTime - previousTimeFPS >= 1.0) {
        framerate = frameCount;

        frameCount = 0;
        previousTimeFPS = currentTime;
    }
}

void clearBackground(float r, float g, float b) {
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void printFPS() {
    printf("%d\n", framerate);
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