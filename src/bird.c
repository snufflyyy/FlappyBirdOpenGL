#include "bird.h"

#include <GLFW/glfw3.h>
#include <stdbool.h>

#include "render.h"
#include "window.h"

Bird createBird(const vec2 position) {
    Bird bird = {0};

    bird.animationSpeed = 0.075f;
    bird.lastAnimationTime = (float) glfwGetTime();
    bird.currentFrameIndex = 0;

    // i think having a texture pointer array would be better to hold the frames
    // (texture) so there isn't any repeated textures like bird.textures[3]

    // create textures
    bird.textures[0] = createTexture("../assets/textures/bird/downflap.png");
    bird.textures[1] = createTexture("../assets/textures/bird/midflap.png");
    bird.textures[2] = createTexture("../assets/textures/bird/upflap.png");
    bird.textures[3] = bird.textures[1];

    bird.sprite = createSprite (
        createShader("../assets/shaders/default.vert", "../assets/shaders/default.frag"),
        bird.textures[0]
    );

    bird.sprite.scale[0] = (float) bird.sprite.texture.width * 2;
    bird.sprite.scale[1] = (float) bird.sprite.texture.height * 2;

    bird.sprite.position[0] = position[0];
    bird.sprite.position[1] = position[1];

    return bird;
}

void updateBird(Bird* bird, const vec2 gravity) {
    // update bird velocity
    bird->velocity[0] -= gravity[0] * deltaTime;
    bird->velocity[1] -= gravity[1] * deltaTime;

    // update bird position
    bird->sprite.position[0] += bird->velocity[0] * deltaTime;
    bird->sprite.position[1] += bird->velocity[1] * deltaTime;

    // bird rotation
    if (bird->velocity[1] > 0 && bird->velocity[1] < 700) {
        bird->sprite.rotation = bird->velocity[1] / 20;
    }
    if (bird->velocity[1] < 0) {
        bird->sprite.rotation = bird->velocity[1] / 20;
    }
}

void animateBird(Bird* bird) {
    if (glfwGetTime() - bird->lastAnimationTime > bird->animationSpeed) {
        bird->currentFrameIndex++;

        if (bird->currentFrameIndex > 3) {
            bird->currentFrameIndex = 0;
        }

        bird->sprite.texture = bird->textures[bird->currentFrameIndex];
        bird->lastAnimationTime = (float) glfwGetTime();
    }
}

void birdJump(Bird* bird, float jumpAmount) {
    bird->velocity[1] = jumpAmount;
}

// temp
bool lastKeyState = false;
void birdInput(Bird* bird) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !lastKeyState) {
        bird->velocity[1] = 800;
        lastKeyState = true;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        lastKeyState = false;
    }
}