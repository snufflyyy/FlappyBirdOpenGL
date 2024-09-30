#include "bird.h"

#include <GLFW/glfw3.h>
#include <stdbool.h>

#include "render.h"
#include "window.h"

Bird createBird(const vec2 position) {
    Bird bird = {0};

    bird.animationSpeed = 0.1f;
    bird.lastAnimationTime = (float) glfwGetTime();
    bird.frameTracker = 0;

    // create textures
    bird.frames[0] = createTexture("../assets/textures/bird/downflap.png");
    bird.frames[1] = createTexture("../assets/textures/bird/midflap.png");
    bird.frames[2] = createTexture("../assets/textures/bird/upflap.png");

    // sets currentFrame to the first frame in the frames array
    bird.currentFrame = &bird.frames[0];

    bird.sprite = createSprite (
        createShader("../assets/shaders/default.vert", "../assets/shaders/default.frag"),
        *bird.currentFrame
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
        bird->frameTracker++;

        if (bird->frameTracker > 3) {
            bird->frameTracker = 0;
        }

        switch (bird->frameTracker) {
            case 0: bird->currentFrame = &bird->frames[0]; break;
            case 1: bird->currentFrame = &bird->frames[1]; break;
            case 2: bird->currentFrame = &bird->frames[2]; break;
            case 3: bird->currentFrame = &bird->frames[1]; break;
            default: bird->currentFrame = &bird->frames[0]; break;
        }

        bird->sprite.texture = *bird->currentFrame;
        bird->lastAnimationTime = (float) glfwGetTime();
    }
}

void birdJump(Bird* bird, float jumpAmount) {
    bird->velocity[1] = jumpAmount;
}