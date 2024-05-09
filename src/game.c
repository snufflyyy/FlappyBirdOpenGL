#include "game.h"

#include <stdlib.h>
#include <time.h>

#include <cglm/cglm.h>

#include "sprite.h"
#include "window.h"
#include "pipe.h"
#include "bird.h"

#define NUMBEROFPIPES 3

// game state
GameState gameState;
int score;

// scroll speed of pipes and ground
float scrollSpeed = 175;

// world gravity
vec2 gravity = {0.0f, 1500.0f};

// sprites
Sprite background = {0};

// background settings
float backgroundScrollSpeed = 0.035f;

// sprites cont.
Sprite mainMenuTitle = {0};

// bird
Bird bird = {0};

// pipes
Pipe pipes[NUMBEROFPIPES] = {0};
int lastSpawnedPipeIndex;

void initBackground();
void initMainMenuTitle();
void initBird();
void initPipes();

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

void initBackground() {
	background = createSprite (
        createShader("../assets/shaders/default.vert", "../assets/shaders/scrolling.frag"), 
        createTexture("../assets/textures/backgrounds/night.png")
    );

	background.scale[0] = ((float) background.texture.width / (float) background.texture.height) * (float) windowHeight;
    background.scale[1] = (float) windowHeight;
    background.position[0] = background.scale[0] - background.scale[0] / 2;
    background.position[1] = (float) windowHeight / 2;
    float backgroundScroll = 0;
}

void initMainMenuTitle() {
	// main menu graphic
    Sprite mainMenu = createSprite (
        createShader("../assets/shaders/default.vert", "../assets/shaders/bird.frag"),
        createTexture("../assets/textures/title.png")
    );

    mainMenu.scale[0] = (float) mainMenu.texture.width * 2;
    mainMenu.scale[1] = (float) mainMenu.texture.height * 2;
    mainMenu.position[0] = (float) windowWidth / 2;
    mainMenu.position[1] = (float) windowHeight - mainMenu.scale[1] * 2;
}

void initBird() {
	// beginning position (main menu)
	createBird((vec2) {(float) windowWidth / 2, (float) windowHeight / 2});
}

void initPipes() {
	for (int i = 0; i < NUMBEROFPIPES; i++) {
		pipes[i] = createPipe (
			(vec2) {
				((float) windowWidth + (float) pipes[i].width / 2) + (i * pipeSpacingBetweenPipes),
				rand() % (int) (windowHeight - pipeSpacingTop + 1 - pipeSpacingBottom) + pipeSpacingBottom
			},
			pipeOpeningSize
		);
	}

	lastSpawnedPipeIndex = NUMBEROFPIPES - 1;
}