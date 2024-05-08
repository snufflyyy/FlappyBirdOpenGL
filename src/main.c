#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "sprite.h"
#include "texture.h"
#include "window.h"
#include "shader.h"
#include "render.h"
#include "bird.h"
#include "pipe.h"

int main() {
	createWindow(432, 768, "Flappy Bird");
	initRenderer();

    srand(time(NULL));

	float backgroundScroll = 0;

    vec2 gravity = {0.0f, 1500.0f};
    float scrollSpeed = 0;

	Sprite background = createSprite (
		createShader("../assets/shaders/default.vert", "../assets/shaders/scrolling.frag"), 
		createTexture("../assets/textures/backgrounds/night.png"),
        (vec2) {(float) windowWidth / 2, (float) windowHeight / 2},
        (vec2) {(float) windowWidth, (float) windowHeight}
	);

    // grounds
    Sprite grounds[2] = {0};
	grounds[0] = createSprite (
		createShader("../assets/shaders/default.vert", "../assets/shaders/bird.frag"),
		createTexture("../assets/textures/ground.png"),
        (vec2) {(float) windowWidth / 2, ((float) windowWidth * 0.33f) / 2},
        (vec2) {(float) windowWidth, (float) windowWidth * 0.33f}
	);
    grounds[1] = createSprite (
        createShader("../assets/shaders/default.vert", "../assets/shaders/bird.frag"),
        createTexture("../assets/textures/ground.png"),
        (vec2) {(float) windowWidth + windowWidth / 2, ((float) windowWidth * 0.33f) / 2},
        (vec2) {(float) windowWidth, (float) windowWidth * 0.33f}
    );

    Sprite pipe = createSprite (
        createShader("../assets/shaders/default.vert", "../assets/shaders/bird.frag"),
        createTexture("../assets/textures/pipe.png"),
        (vec2) {(float) windowWidth / 2, (float) windowWidth / 2},
        (vec2) {(float) 52 * 2, (float) 320 * 2}
    );

    Bird bird = createBird((vec2) {(float) windowWidth / 2 - 100, (float) windowHeight / 2});

	while (!glfwWindowShouldClose(window)) {
		backgroundScroll += 0.025f * deltaTime;

        for (int i = 0; i < 2; i++) {
            grounds[i].position[0] -= 100 * deltaTime;
        }

        for (int i = 0; i < 2; i++) {
            if (grounds[i].position[0] < -windowWidth / 2) {
                grounds[i].position[0] = windowWidth + windowWidth / 2;
            }
        }

        pipe.position[0] -= 100 * deltaTime;

        if (pipe.position[0] < -100) {
            pipe.position[0] = windowWidth + 100;
        }

        birdInput(&bird);
        updateBird(&bird, gravity);

		beginRendering();
			clearBackground(0.0f, 0.0f, 0.0f);
			renderSprite(background);
			glUniform1f(glGetUniformLocation(background.shader.id, "scrollSpeed"), backgroundScroll);

            renderSprite(pipe);
            for (int i = 0; i < 2; i++) {
                renderSprite(grounds[i]);
            }
			renderSprite(bird.sprite);
		endRendering();	
	}
	
	// clean up
	deinitRenderer();

	glfwTerminate();
	return 0;
}