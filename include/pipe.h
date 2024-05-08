#pragma once

#include <cglm/types.h>

#include "sprite.h"

typedef struct {
    vec2 position;
    float openingSize;

    Sprite topPipe;
    Sprite bottomPipe;
} Pipe;

Pipe createPipe(vec2 position, float openingSize, Shader shader, Texture texture);
void renderPipe(Pipe pipe);