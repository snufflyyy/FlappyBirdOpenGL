#pragma once

typedef struct {
    unsigned int id;
} Shader;

extern int framerate;

Shader createShader(const char* vertexSourcePath, const char* fragmentSourcePath);
void useShader(Shader shader);
void deleteShader(Shader *shader);