#include "game.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <glad/glad.h>
#include <stdio.h>

#include "GLFW/glfw3.h"
#include "sprite.h"
#include "window.h"
#include "render.h"
#include "pipe.h"
#include "bird.h"

#define NUMBEROFPIPES 3

// if the just key is pressed
bool lastPressState;
bool mainKeyJustPressed;

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
vec2 gravity = {0.0f, 2000.0f};

// default shader
Shader defaultShader = {0};

// sprites
Sprite background = {0};
float backgroundScroll;

Sprite mainMenuTitle = {0};
Sprite gameOverText = {0};

// bird
Bird bird = {0};

// pipes
Pipe pipes[NUMBEROFPIPES] = {0};
int lastSpawnedPipeIndex;

// score counter
Sprite scoreCounters[3] = {0};

// number textures
Texture numbers[10];

static void initBackground();
static void updateBackground();
static void initMainMenuTitle();
static void initGameOverText();
static void initBird();
static void resetBird();
static void initPipes();
static void resetPipes();
static void updatePipes();
static void renderPipes();
static void initScoreCounters();
static void updateScoreCounters();
static void renderScoreCounters();

static void getMainKeyInput() {
    if (lastPressState) {
        mainKeyJustPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !lastPressState) {
        mainKeyJustPressed = true;
        lastPressState = true;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        lastPressState = false;
    }
}

void initGame() {
	// set random seed
    srand(time(NULL));

	// set game state to main menu on launch
	gameState = MAINMENU;

    // create default shader
    defaultShader = createShader("../assets/shaders/default.vert", "../assets/shaders/default.frag");

    // load textures
    for (int i = 0; i < 10; i++) {
        char file[10];
        sprintf(file, "%d", i);
        char path[100] = "../assets/textures/numbers/";
        strcat(path, file);
        strcat(path, ".png");
        numbers[i] = createTexture(path);
    }

	// init sprites
	initBackground();
	initMainMenuTitle();
    initScoreCounters();
    initGameOverText();

	initBird();
	initPipes();
}

void gameUpdate() {
    getMainKeyInput();

    switch (gameState) {
        case MAINMENU:
            if (mainKeyJustPressed) {
                gameState = GAMEPLAY;
            }

            updateBackground();
            animateBird(&bird);
            break;
        case GAMEPLAY:
            if (mainKeyJustPressed) {
                birdJump(&bird, 600.0f);
            }
            updateBackground();
            updatePipes();
            updateScoreCounters();
            updateBird(&bird, gravity);
            animateBird(&bird);
            break;
        case GAMEOVER:
            if (mainKeyJustPressed) {
                resetPipes();
                resetBird();

                score = 0;

                gameState = GAMEPLAY;
            }

            updateBird(&bird, gravity);
            break;
    }
}

void gameRender() {
    renderSprite(background);
    glUniform1f(glGetUniformLocation(background.shader.id, "scrollSpeed"), backgroundScroll);

    switch (gameState) {
        case MAINMENU:
            renderSprite(bird.sprite);
			renderSprite(mainMenuTitle);
            break;
        case GAMEPLAY:
            renderPipes();
			renderSprite(bird.sprite);
            renderScoreCounters();
            break;
        case GAMEOVER:
			renderPipes();
			renderSprite(bird.sprite);

            renderSprite(gameOverText);
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
        defaultShader,
        createTexture("../assets/textures/title.png")
    );

    mainMenuTitle.scale[0] = (float) mainMenuTitle.texture.width * 2;
    mainMenuTitle.scale[1] = (float) mainMenuTitle.texture.height * 2;
    mainMenuTitle.position[0] = (float) windowWidth / 2;
    mainMenuTitle.position[1] = (float) windowHeight - mainMenuTitle.scale[1] * 2;
}

static void initGameOverText() {
    // main menu graphic
    gameOverText = createSprite (
        defaultShader,
        createTexture("../assets/textures/gameover.png")
    );

    gameOverText.scale[0] = (float) gameOverText.texture.width * 2;
    gameOverText.scale[1] = (float) gameOverText.texture.height * 2;
    gameOverText.position[0] = (float) windowWidth / 2;
    gameOverText.position[1] = (float) windowHeight - mainMenuTitle.scale[1] * 2;
}

static void initBird() {
	bird = createBird((vec2) {(float) windowWidth / 2, (float) windowHeight / 2});
}

static void resetBird() {
    bird.sprite.position[0] = (float) windowWidth / 2;
    bird.sprite.position[1] = (float) windowHeight / 2;

    bird.velocity[0] = 0;
    bird.velocity[1] = 800;
}

static void initPipes() {
	for (int i = 0; i < NUMBEROFPIPES; i++) {
        // position is set to this so it doesn't flash, this is a lazy solution but fuck it
		pipes[i] = createPipe ((vec2) {(float) windowWidth * 2, (float) windowHeight * 2}, 200);
	}

    resetPipes();

	lastSpawnedPipeIndex = NUMBEROFPIPES - 1;
}

static void resetPipes() {
    for (int i = 0; i < NUMBEROFPIPES; i++) {
        pipes[i].position[0] = ((float) windowWidth + (float) pipes[i].scale[0] / 2) + (float) (i * 300);
        pipes[i].position[1] = rand() % (int) (windowHeight - 250 + 1 - 300) + 300;

        pipes[i].passed = false;
    }

    lastSpawnedPipeIndex = NUMBEROFPIPES - 1;
}

static void updatePipes() {
    for (int i = 0; i < NUMBEROFPIPES; i++) {
        // "spawn" new pipe if out of bounds
        if (pipes[i].position[0] < -100) {
            pipes[i].position[0] = pipes[lastSpawnedPipeIndex].position[0] + 300;
            pipes[i].position[1] = rand() % ((windowHeight - 250 + 1) - 250) + 250;

            pipes[i].passed = false;
            lastSpawnedPipeIndex = i;
        }

        // check if the bird touches the pipe
        if (getSpriteCollision(&bird.sprite, &pipes[i].bottomPipe) || getSpriteCollision(&bird.sprite, &pipes[i].topPipe)) {
            gameState = GAMEOVER;

            bird.velocity[0] = -100;
            bird.velocity[1] = 200;
        }

        // check if the bird passes through
        if (getSpriteTriggerCollision(&bird.sprite, &pipes[i].trigger) && !pipes[i].passed) {
            pipes[i].passed = true;

            score++;
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

static void initScoreCounters() {
    for (int i = 0; i < 3; i++) {
        scoreCounters[i] = createSprite(defaultShader, numbers[0]);

        scoreCounters[i].scale[0] = (float) scoreCounters[i].texture.width * 1.5f;
        scoreCounters[i].scale[1] = (float) scoreCounters[i].texture.height * 1.5f;

        scoreCounters[i].position[0] = (float) windowWidth / 2;
        scoreCounters[i].position[1] = (float) windowHeight - (float) scoreCounters[i].texture.height * 2;
    }
}

static void updateScoreCounters() {
    if (score < 10) {
        scoreCounters[2].scale[0] = (float) scoreCounters[2].texture.width * 1.5f;
        scoreCounters[2].scale[1] = (float) scoreCounters[2].texture.height * 1.5f;

        scoreCounters[2].position[0] = (float) windowWidth / 2;
        scoreCounters[2].position[1] = (float) windowHeight - (float) scoreCounters[2].texture.height * 2;

        scoreCounters[2].texture = numbers[score];
    }
}

static void renderScoreCounters() {
    if (score < 10) {
        renderSprite(scoreCounters[2]);
    } else if (score < 100) {
        scoreCounters[1].position[0] = (float) windowWidth / 2 - (float) scoreCounters[1].texture.width / 1.5f;
        scoreCounters[2].position[0] = (float) windowWidth / 2 + (float) scoreCounters[2].texture.width / 1.5f;

        renderSprite(scoreCounters[1]);
        renderSprite(scoreCounters[2]);
    } else {

    }
}