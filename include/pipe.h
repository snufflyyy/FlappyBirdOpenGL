#pragma once

#include <cglm/types.h>

#include "sprite.h"

typedef struct {
    vec2 position;
    float openingSize;

    int width;
    int height;

    Sprite topPipe;
    Sprite bottomPipe;
} Pipe;

Pipe createPipe(const vec2 position, float openingSize);
void updatePipe(Pipe *pipe);
void renderPipe(Pipe pipe);