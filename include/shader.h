#pragma once

typedef struct {
    unsigned int id;
} Shader;

Shader createShader(const char* vertexSourcePath, const char* fragmentSourcePath);
void useShader(Shader shader);
void deleteShader(Shader *shader);