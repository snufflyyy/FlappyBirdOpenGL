#include "game.h"

#include <stdlib.h>
#include <time.h>

#include <glad/glad.h>
#include <cglm/cglm.h>

#include "GLFW/glfw3.h"
#include "sprite.h"
#include "window.h"
#include "render.h"
#include "pipe.h"
#include "bird.h"

#define NUMBEROFPIPES 3

typedef enum {
    MAINMENU,
    GAMEPLAY,
    GAMEOVER
} GameState;

// game state
GameState gameState;
int score;

// scroll speed of pipes and ground
float scrollSpeed = 175;

// world gravity
vec2 gravity = {0.0f, 1500.0f};

// sprites
Sprite background = {0};
float backgroundScroll;

// sprites cont.
Sprite mainMenuTitle = {0};

// bird
Bird bird = {0};

// pipes
Pipe pipes[NUMBEROFPIPES] = {0};
int lastSpawnedPipeIndex;

static void initBackground();
static void updateBackground();
static void initMainMenuTitle();
static void initBird();
static void initPipes();
static void updatePipes();
static void renderPipes();

void initGame() {
	// set random seed
    srand(time(NULL));

	// set game state to main menu on launch
	gameState = MAINMENU;

	// init sprites
	initBackground();
	initMainMenuTitle();

	initBird();
	initPipes();
}

void gameUpdate() {
    switch (gameState) {
        case MAINMENU:
            updateBackground();
            animateBird(&bird);

			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
				gameState = GAMEPLAY;
			}

            break;
        case GAMEPLAY:
            updateBackground();
            birdInput(&bird);
            updateBird(&bird, gravity);
            animateBird(&bird);

            updatePipes();

            break;
        case GAMEOVER:
            updateBird(&bird, gravity);

			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
				gameState = GAMEPLAY;
			}
            break;
    }
}

void gameRender() {
    switch (gameState) {
        case MAINMENU:
            renderSprite(background);
            glUniform1f(glGetUniformLocation(background.shader.id, "scrollSpeed"), backgroundScroll);
            renderSprite(bird.sprite);
			renderSprite(mainMenuTitle);
            break;
        case GAMEPLAY:
            renderSprite(background);
            glUniform1f(glGetUniformLocation(background.shader.id, "scrollSpeed"), backgroundScroll);
            
            renderPipes();

			renderSprite(bird.sprite);
            break;
        case GAMEOVER:
		    renderSprite(background);
            glUniform1f(glGetUniformLocation(background.shader.id, "scrollSpeed"), backgroundScroll);
            
			renderPipes();

			renderSprite(bird.sprite);
            break;
    }
}

static void initBackground() {
	background = createSprite (
        createShader("../assets/shaders/default.vert", "../assets/shaders/scrolling.frag"), 
        createTexture("../assets/textures/backgrounds/night.png")
    );

	background.scale[0] = ((float) background.texture.width / (float) background.texture.height) * (float) windowHeight;
    background.scale[1] = (float) windowHeight;
    background.position[0] = background.scale[0] - background.scale[0] / 2;
    background.position[1] = (float) windowHeight / 2;
}

static void updateBackground() {
    backgroundScroll += 0.035f * deltaTime;
}

static void initMainMenuTitle() {
	// main menu graphic
    mainMenuTitle = createSprite (
        createShader("../assets/shaders/default.vert", "../assets/shaders/default.frag"),
        createTexture("../assets/textures/title.png")
    );

    mainMenuTitle.scale[0] = (float) mainMenuTitle.texture.width * 2;
    mainMenuTitle.scale[1] = (float) mainMenuTitle.texture.height * 2;
    mainMenuTitle.position[0] = (float) windowWidth / 2;
    mainMenuTitle.position[1] = (float) windowHeight - mainMenuTitle.scale[1] * 2;
}

static void initBird() {
	// beginning position (main menu)
	bird = createBird((vec2) {(float) windowWidth / 2, (float) windowHeight / 2});
}

static void initPipes() {
	for (int i = 0; i < NUMBEROFPIPES; i++) {
		pipes[i] = createPipe (
			(vec2) {
				((float) windowWidth + (float) pipes[i].width / 2) + (i * 300), // 300 is spacing between pipes
                // (windowHeight - pipe spacing top + 1 - pipe spacing bottom) + pipe spacing bottom
				rand() % (int) (windowHeight - 250 + 1 - 300) + 300
			},
            // opening size
			250
		);
	}

	lastSpawnedPipeIndex = NUMBEROFPIPES - 1;
}

static void updatePipes() {
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

static void renderPipes() {
    for (int i = 0; i < NUMBEROFPIPES; i++) {
        renderPipe(pipes[i]);
    }
}