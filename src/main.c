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

#define NUMBEROFPIPES 3

typedef enum {
    MAINMENU,
    GAMEPLAY,
    GAMEOVER
} GameState;

// game state / related stuff
GameState gameState = MAINMENU;
int score;

int main() {
	createWindow(432, 768, "Flappy Bird");
	initRenderer();
    srand(time(NULL));

    float scrollSpeed = 175;

    vec2 gravity = {0.0f, 1500.0f};

    // background
    Sprite background = createSprite (
        createShader("../assets/shaders/default.vert", "../assets/shaders/scrolling.frag"), 
        createTexture("../assets/textures/backgrounds/night.png")
    );
    background.scale[0] = ((float) background.texture.width / (float) background.texture.height) * (float) windowHeight;
    background.scale[1] = (float) windowHeight;
    background.position[0] = background.scale[0] - background.scale[0] / 2;
    background.position[1] = (float) windowHeight / 2;
    float backgroundScroll = 0;

    // main menu graphic
    Sprite mainMenu = createSprite (
        createShader("../assets/shaders/default.vert", "../assets/shaders/bird.frag"),
        createTexture("../assets/textures/title.png")
    );

    mainMenu.scale[0] = (float) mainMenu.texture.width * 2;
    mainMenu.scale[1] = (float) mainMenu.texture.height * 2;
    mainMenu.position[0] = (float) windowWidth / 2;
    mainMenu.position[1] = (float) windowHeight - mainMenu.scale[1] * 2;

    // number textures
    Texture numberZero = createTexture("../assets/textures/numbers/0.png");
    Texture numberOne = createTexture("../assets/textures/numbers/1.png");
    Texture numberTwo = createTexture("../assets/textures/numbers/2.png");
    Texture numberThree = createTexture("../assets/textures/numbers/3.png");
    Texture numberFour = createTexture("../assets/textures/numbers/4.png");
    Texture numberFive = createTexture("../assets/textures/numbers/5.png");
    Texture numberSix = createTexture("../assets/textures/numbers/6.png");
    Texture numberSeven = createTexture("../assets/textures/numbers/7.png");
    Texture numberEight = createTexture("../assets/textures/numbers/8.png");
    Texture numberNine = createTexture("../assets/textures/numbers/9.png");

    // score counter
    Sprite scoreCounter[3] = {0};

    for (int i = 0; i < 3; i++) {
        scoreCounter[i] = createSprite (
            createShader("../assets/shaders/default.vert", "../assets/shaders/bird.frag"),
            numberZero
        );

        scoreCounter[i].scale[0] = scoreCounter[i].texture.width * 1.5f;
        scoreCounter[i].scale[1] = scoreCounter[i].texture.height * 1.5f;
        scoreCounter[i].position[0] = (windowWidth / 2) + (i - 1) * scoreCounter[i].scale[0];
        scoreCounter[i].position[1] = windowHeight / 2 + 250;
    }

    // pipes
    Pipe pipes[3] = {0};
    int lastSpawnedPipeIndex = 2;

    Bird bird = createBird((vec2) {(float) windowWidth / 2, (float) windowHeight / 2});

	while (!glfwWindowShouldClose(window)) {
        // update

        // how fast the background moves
        if (gameState != GAMEOVER) {
            backgroundScroll += 0.035f * deltaTime;
            animateBird(&bird);
        }

        if (gameState != GAMEPLAY) {
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                // spawn pipes
                for (int i = 0; i < NUMBEROFPIPES; i++) {
                    pipes[i] = createPipe (
                        (vec2) {
                            (float) ((float) windowWidth + (float) pipes[i].width / 2) + (float) (i * 300),
                            rand() % ((windowHeight - 250 + 1) - 250) + 250
                        },
                        250.0f
                    );
                }

                // set bird position
                bird.sprite.position[0] = (float) windowWidth / 2 - 100;

                gameState = GAMEPLAY;
            }
        }

        if (gameState != MAINMENU) {
            updateBird(&bird, gravity);
        }

        if (gameState == GAMEPLAY) {
            birdInput(&bird);

            // update tiles
            for (int i = 0; i < NUMBEROFPIPES; i++) {
                // "spawn" new pipe if out of bounds
                if (pipes[i].position[0] < -100) {
                    pipes[i].position[0] = pipes[lastSpawnedPipeIndex].position[0] + 300;
                    pipes[i].position[1] = rand() % ((windowHeight - 250 + 1) - 250) + 250;

                    lastSpawnedPipeIndex = i;
                }

                // check if the bird touches the pipe
                if (getCollision(&bird.sprite, &pipes[i].bottomPipe) || getCollision(&bird.sprite, &pipes[i].topPipe)) {
                    gameState = GAMEOVER;

                    bird.velocity[0] = -50;
                    bird.velocity[1] = 200;
                }

                pipes[i].position[0] -= scrollSpeed * deltaTime;

                updatePipe(&pipes[i]);
            }
        }

        // render
		beginRendering();
			clearBackground(0.0f, 0.0f, 0.0f);

            renderSprite(background);
            glUniform1f(glGetUniformLocation(background.shader.id, "scrollSpeed"), backgroundScroll);

            if (gameState == MAINMENU) {
                renderSprite(mainMenu);
            }

            if (gameState != MAINMENU) {
                for (int i = 0; i < NUMBEROFPIPES; i++) {
                    renderPipe(pipes[i]);

                    for (int i = 0; i < 3; i++) {
                        if (score < 10) {
                            renderSprite(scoreCounter[1]);
                        }
                    }
                }
            }

            renderSprite(bird.sprite);
		endRendering();	
	}
	
	// clean up
	deinitRenderer();

	glfwTerminate();
	return 0;
}