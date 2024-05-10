#include "pipe.h"
#include "shader.h"
#include "sprite.h"
#include "texture.h"

Pipe createPipe(const vec2 position, float openingSize) {
    Pipe pipe = {0};

    Shader pipeShader = createShader("../assets/shaders/default.vert", "../assets/shaders/default.frag");
    Texture pipeTexture = createTexture("../assets/textures/pipe.png");

    pipe.scale[0] = (float) pipeTexture.width * 2;
    pipe.scale[1] = (float) pipeTexture.height * 2;

    pipe.position[0] = position[0];
    pipe.position[1] = position[1];

    pipe.openingSize = openingSize;

    pipe.trigger.position[0] = position[0];
    pipe.trigger.position[1] = position[1];
    pipe.trigger.scale[0] = pipe.scale[0] / 2;
    pipe.trigger.scale[1] = openingSize * 2;

    pipe.topPipe = createSprite(pipeShader, pipeTexture);
    pipe.topPipe.scale[0] = pipe.scale[0];
    pipe.topPipe.scale[1] = pipe.scale[1];
    pipe.topPipe.position[0] = position[0];
    pipe.topPipe.position[1] = position[1] + openingSize / 2 + pipe.topPipe.scale[1] / 2;
    pipe.topPipe.rotation = 180;

    pipe.bottomPipe = createSprite(pipeShader, pipeTexture);
    pipe.bottomPipe.scale[0] = pipe.scale[0];
    pipe.bottomPipe.scale[1] = pipe.scale[1];
    pipe.bottomPipe.position[0] = position[0];
    pipe.bottomPipe.position[1] = position[1] - openingSize / 2 - pipe.bottomPipe.scale[1] / 2;

    return pipe;
}

bool getSpriteTriggerCollision(Sprite *sprite, Trigger *trigger) {
    bool collisionX = sprite->position[0] - sprite->scale[0] / 2 + sprite->scale[0] >= trigger->position[0] - trigger->scale[0] / 2 &&
                      trigger->position[0] - trigger->scale[0] / 2 + trigger->scale[0] >= sprite->position[0] - sprite->scale[0] / 2;

    bool collisionY = sprite->position[1] - sprite->scale[1] / 2 + sprite->scale[1] >= trigger->position[1] - trigger->scale[1] / 2 &&
                      trigger->position[1] - trigger->scale[1] / 2 + trigger->scale[1] >= sprite->position[1] - sprite->scale[1] / 2;

    return collisionX && collisionY;
}

void updatePipe(Pipe *pipe) {
    pipe->trigger.position[0] = pipe->position[0];
    pipe->trigger.position[1] = pipe->position[1];

    pipe->topPipe.position[0] = pipe->position[0];
    pipe->topPipe.position[1] = pipe->position[1] + pipe->openingSize / 2 + pipe->topPipe.scale[1] / 2;;

    pipe->bottomPipe.position[0] = pipe->position[0];
    pipe->bottomPipe.position[1] = pipe->position[1] - pipe->openingSize / 2 - pipe->bottomPipe.scale[1] / 2;
}

void renderPipe(Pipe pipe) {
    renderSprite(pipe.topPipe);
    renderSprite(pipe.bottomPipe);
}