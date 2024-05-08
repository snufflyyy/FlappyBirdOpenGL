#include "pipe.h"

Pipe createPipe(vec2 position, float openingSize, Shader shader, Texture texture) {
    Pipe pipe;

    pipe.position[0] = position[0];
    pipe.position[1] = position[1];

    pipe.openingSize = openingSize;

    pipe.topPipe = createSprite(
        shader,
        texture,
        (vec2) {position[0], position[1]},
        (vec2) {0.0f, 0.0f}
    );

    return pipe;
}