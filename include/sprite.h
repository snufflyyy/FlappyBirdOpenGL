#pragma once

#include <stdbool.h>

#include <cglm/types.h>

#include "shader.h"
#include "texture.h"

typedef struct {
	vec2 position;
	vec2 scale;
	float rotation;

	mat4 transform;

	Shader shader;
	Texture texture;
} Sprite;

Sprite createSprite(Shader shader, Texture texture);
bool getCollision(Sprite *spriteOne, Sprite *spriteTwo);
void renderSprite(Sprite sprite);
void deleteSprite(Sprite *sprite);