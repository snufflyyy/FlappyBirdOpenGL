#include "bird.h"

#include <GLFW/glfw3.h>

#include "render.h"
#include "window.h"

Bird createBird(const vec2 position) {
    Bird bird = {0};

    bird.animationSpeed = 0.075f;
    bird.lastAnimationTime = (float) glfwGetTime();
    bird.currentFrameIndex = 0;

    // create textures
    bird.textures[0] = createTexture("../assets/textures/bird/downflap.png");
    bird.textures[1] = createTexture("../assets/textures/bird/midflap.png");
    bird.textures[2] = createTexture("../assets/textures/bird/upflap.png");
    bird.textures[3] = bird.textures[1];

    bird.sprite = createSprite (
        createShader("../assets/shaders/default.vert", "../assets/shaders/bird.frag"),
        bird.textures[0],
        (vec2) {position[0], position[1]},
        (vec2) {68, 48}
    );

    return bird;
}

void updateBird(Bird* bird, const vec2 gravity) {
    // update bird velocity
    bird->velocity[0] -= gravity[0] * deltaTime;
    bird->velocity[1] -= gravity[1] * deltaTime;

    // update bird position
    bird->sprite.position[0] += bird->velocity[0] * deltaTime;
    bird->sprite.position[1] += bird->velocity[1] * deltaTime;

    // bird animation
    if (glfwGetTime() - bird->lastAnimationTime > bird->animationSpeed) {
        bird->currentFrameIndex++;

        if (bird->currentFrameIndex > 3) {
            bird->currentFrameIndex = 0;
        }

        bird->sprite.texture = bird->textures[bird->currentFrameIndex];
        bird->lastAnimationTime = glfwGetTime();
    }

    // bird rotation
    if (bird->velocity[1] > 0 && bird->velocity[1] < 500) {
        bird->sprite.rotation = bird->velocity[1] / 20;
    }
    if (bird->velocity[1] < 0 && bird->velocity[1] > -500) {
        bird->sprite.rotation = bird->velocity[1] / 20;
    }
}

void birdInput(Bird* bird) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        bird->velocity[1] = 400;
    }
}