#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "sprite.h"
#include "texture.h"
#include "window.h"
#include "shader.h"
#include "render.h"

int main() {
	createWindow(432, 768, "Flappy Bird");
	initRenderer();

	float backgroundScroll = 0;
	float groundScroll = 0;

	Sprite background = createSprite (
		(vec2) {(float) windowWidth / 2, (float) windowHeight / 2}, 
		(vec2) {windowWidth, windowHeight}, 
		createShader("../assets/shaders/default.vert", "../assets/shaders/scrolling.frag"), 
		createTexture("../assets/textures/backgrounds/night.png")
	);

	Sprite ground = createSprite (
		(vec2) {(float) windowWidth / 2, 72}, 
		(vec2) {432, 144}, 
		createShader("../assets/shaders/default.vert", "../assets/shaders/scrolling.frag"), 
		createTexture("../assets/textures/ground.png")
	);

	Sprite bird = createSprite (
		(vec2) {(float) windowWidth / 2 - 100, (float) windowHeight / 2}, 
		(vec2) {68, 48}, 
		createShader("../assets/shaders/default.vert", "../assets/shaders/bird.frag"), 
		createTexture("../assets/textures/bird/midflap.png")
	);

	while (!glfwWindowShouldClose(window)) {
		backgroundScroll += 0.00015f;
		groundScroll += 0.002f;

		printFPS();

		beginRendering();
			clearBackground(0.2f, 0.2f, 0.2f);
			renderSprite(background);
			glUniform1f(glGetUniformLocation(background.shader.id, "scrollSpeed"), backgroundScroll);

			renderSprite(ground);
			glUniform1f(glGetUniformLocation(ground.shader.id, "scrollSpeed"), groundScroll);

			renderSprite(bird);
		endRendering();	
	}
	
	// clean up
	deinitRenderer();

	glfwTerminate();
	return 0;
}