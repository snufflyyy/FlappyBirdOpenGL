#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include "render.h"
#include "game.h"

int main() {
	createWindow(432, 768, "Flappy Bird");
	initRenderer();

    initGame();

	while (!glfwWindowShouldClose(window)) {
        // update
        gameUpdate();

        // render
		beginRendering();
			clearBackground(0.0f, 0.0f, 0.0f);
            gameRender();
		endRendering();	
	}
	
	// clean up
	deinitRenderer();

	glfwTerminate();
	return 0;
}