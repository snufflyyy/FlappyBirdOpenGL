#pragma once

#include <cglm/types.h>

#include "sprite.h"

// maybe don't have it here past braden?
typedef struct {
    vec2 position;
    vec2 scale;
} Trigger;

typedef struct {
    vec2 position;
    vec2 scale;

    float openingSize;

    Trigger trigger;
    bool passed;

    Sprite topPipe;
    Sprite bottomPipe;
} Pipe;

Pipe createPipe(const vec2 position, float openingSize);
bool getSpriteTriggerCollision(Sprite *sprite, Trigger *trigger);
void updatePipe(Pipe *pipe);
void renderPipe(Pipe pipe);