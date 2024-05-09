#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "sprite.h"
#include "texture.h"
#include "window.h"
#include "shader.h"
#include "render.h"
#include <game.h>

int main() {
	createWindow(432, 768, "Flappy Bird");
	initRenderer();

    initGame();

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

                // set bird position
                bird.sprite.position[0] = (float) windowWidth / 2 - 100;
                bird.sprite.position[1] = (float) windowHeight / 2;

                lastSpawnedPipeIndex = 2;

                // reset bird velocity
                bird.velocity[0] = 0;
                bird.velocity[1] = 0;

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