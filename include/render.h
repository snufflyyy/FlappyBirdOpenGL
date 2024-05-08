#pragma once

#include <cglm/types.h>

extern unsigned int VAO, VBO, EBO;
extern mat4 projection;

void initRenderer();
void beginRendering();
void clearBackground(float r, float g, float b);
void printFPS();
void endRendering();
void deinitRenderer();