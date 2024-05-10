#pragma once

#include "sprite.h"
#include "texture.h"

typedef struct {
    Sprite sprite;
    vec2 velocity;

    float animationSpeed;
    float lastAnimationTime;

    Texture textures[4];
    int currentFrameIndex;
} Bird;

Bird createBird(const vec2 position);

void updateBird(Bird* bird, const vec2 gravity);
void animateBird(Bird* bird);
void birdJump(Bird* bird, float jumpAmount);