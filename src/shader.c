#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static char* loadFile(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Failed to load File!\n");
    }

    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // prevents random ass letters and symbols showing up in the string
    char* vertexSource = malloc(length + 1); // this
    vertexSource[0] = '\0'; // and this

    // don't input more than 256 characters <:( (unlikely)
    char buffer[256];
    while (fgets(buffer, 256, file)) {
        strcat(vertexSource, buffer);
    }

    fclose(file);

    return vertexSource;
}

static unsigned int createIndividualShader(const char* source, int type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        printf("Failed to compile Shader!\n");
    }

    return shader;
}

Shader createShader(const char* vertexSourcePath, const char* fragmentSourcePath) {
    Shader shader = {0};

    char* vertexSource = loadFile(vertexSourcePath);
    char* fragmentSource = loadFile(fragmentSourcePath);

    unsigned int vertexShader = createIndividualShader(vertexSource, GL_VERTEX_SHADER);
    unsigned int fragmentShader = createIndividualShader(fragmentSource, GL_FRAGMENT_SHADER);

    shader.id = glCreateProgram();
    glAttachShader(shader.id, vertexShader);
    glAttachShader(shader.id, fragmentShader);
    glLinkProgram(shader.id);

    // clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    free(vertexSource);
    free(fragmentSource);

    return shader;
}

void useShader(Shader shader) {
    glUseProgram(shader.id);
}

void deleteShader(Shader *shader) {
    glDeleteProgram(shader->id);
}