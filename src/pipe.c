#include <stdlib.h>
#include "pipe.h"
#include "shader.h"
#include "sprite.h"
#include "texture.h"
#include "window.h"

Pipe createPipe(const vec2 position, float openingSize) {
    Pipe pipe = {0};

    Shader pipeShader = createShader("../assets/shaders/default.vert", "../assets/shaders/default.frag");
    Texture pipeTexture = createTexture("../assets/textures/pipe.png");

    pipe.width = pipeTexture.width * 2;
    pipe.height = pipeTexture.height * 2;

    pipe.position[0] = position[0];
    pipe.position[1] = position[1];

    pipe.openingSize = openingSize;

    pipe.topPipe = createSprite(pipeShader, pipeTexture);
    pipe.topPipe.scale[0] = pipe.topPipe.texture.width * 2;
    pipe.topPipe.scale[1] = pipe.topPipe.texture.height * 2;
    pipe.topPipe.position[0] = position[0];
    pipe.topPipe.position[1] = position[1] + openingSize / 2 + pipe.topPipe.scale[1] / 2;
    pipe.topPipe.rotation = 180;

    pipe.bottomPipe = createSprite(pipeShader, pipeTexture);
    pipe.bottomPipe.scale[0] = pipe.bottomPipe.texture.width * 2;
    pipe.bottomPipe.scale[1] = pipe.bottomPipe.texture.height * 2;
    pipe.bottomPipe.position[0] = position[0];
    pipe.bottomPipe.position[1] = position[1] - openingSize / 2 - pipe.bottomPipe.scale[1] / 2;

    return pipe;
}

void updatePipe(Pipe *pipe) {
    pipe->topPipe.position[0] = pipe->position[0];
    pipe->topPipe.position[1] = pipe->position[1] + pipe->openingSize / 2 + pipe->topPipe.scale[1] / 2;;

    pipe->bottomPipe.position[0] = pipe->position[0];
    pipe->bottomPipe.position[1] = pipe->position[1] - pipe->openingSize / 2 - pipe->bottomPipe.scale[1] / 2;
}

void renderPipe(Pipe pipe) {
    renderSprite(pipe.topPipe);
    renderSprite(pipe.bottomPipe);
}