#pragma once

#include <cglm/types.h>

#include "shader.h"
#include "texture.h"

extern const float vertices[16];
extern const unsigned int indices[6];

typedef struct {
	vec2 position;
	vec2 scale;
	float rotation;

	mat4 transform;

	Shader shader;
	Texture texture;
} Sprite;

Sprite createSprite(Shader shader, Texture texture ,vec2 position, vec2 scale);
void renderSprite(Sprite sprite);