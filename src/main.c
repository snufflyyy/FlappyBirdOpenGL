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

int main() {
	createWindow(432, 768, "Flappy Bird");
	initRenderer();

    srand(time(NULL));

	float backgroundScroll = 0;
    float scrollSpeed = 200;

    vec2 gravity = {0.0f, 1500.0f};

    // background
    Sprite background = createSprite (
        createShader("../assets/shaders/default.vert", "../assets/shaders/scrolling.frag"), 
        createTexture("../assets/textures/backgrounds/night.png"),
        (vec2) {0, 0},
        (vec2) {0, 0}
    );

    background.scale[0] = ((float) background.texture.width / (float) background.texture.height) * windowHeight;
    background.scale[1] = windowHeight;

    background.position[0] = background.scale[0] - background.scale[0] / 2;
    background.position[1] = (float) windowHeight / 2;

    // grounds
    Sprite grounds[2] = {0};

    for (int i = 0; i < 2; i++) {
        grounds[i] = createSprite (
            createShader("../assets/shaders/default.vert", "../assets/shaders/bird.frag"),
            createTexture("../assets/textures/ground.png"),
            (vec2) {0, 0},
            (vec2) {0,0}
        );

        grounds[i].scale[0] = windowWidth;
        grounds[i].scale[1] = ((float) grounds[i].texture.height / (float) grounds[i].texture.width) * windowWidth;
    }

    // ground one
    grounds[0].position[0] = (float) windowWidth / 2;
    grounds[0].position[1] = grounds[0].scale[1] / 2;

    // ground two
    grounds[1].position[0] = (float) windowWidth / 2 + windowWidth;
    grounds[1].position[1] = grounds[1].scale[1] / 2;


    Sprite pipe = createSprite (
        createShader("../assets/shaders/default.vert", "../assets/shaders/bird.frag"),
        createTexture("../assets/textures/pipe.png"),
        (vec2) {(float) windowWidth / 2, (float) windowWidth / 2},
        (vec2) {(float) 52 * 2, (float) 320 * 2}
    );

    Bird bird = createBird((vec2) {(float) windowWidth / 2 - 100, (float) windowHeight / 2});

	while (!glfwWindowShouldClose(window)) {
		backgroundScroll += 0.035f * deltaTime;

        // ground scrolling
        for (int i = 0; i < 2; i++) {
            grounds[i].position[0] -= scrollSpeed * deltaTime;
        }
        for (int i = 0; i < 2; i++) {
            if (grounds[i].position[0] < (float) -windowWidth / 2) {
                grounds[i].position[0] = (float) windowWidth / 2 + windowWidth;
            }
        }

        pipe.position[0] -= scrollSpeed * deltaTime;

        if (pipe.position[0] < -100) {
            pipe.position[0] = windowWidth + 100;
        }

        birdInput(&bird);
        updateBird(&bird, gravity);

        printFPS();

		beginRendering();
			clearBackground(0.0f, 0.0f, 0.0f);
            renderSprite(background);
            glUniform1f(glGetUniformLocation(background.shader.id, "scrollSpeed"), backgroundScroll);

            for (int i = 0; i < 2; i++) {
                renderSprite(grounds[i]);
            }

            renderSprite(bird.sprite);

            renderSprite(pipe);
            for (int i = 0; i < 2; i++) {
                renderSprite(grounds[i]);
            }
		endRendering();	
	}
	
	// clean up
	deinitRenderer();

	glfwTerminate();
	return 0;
}