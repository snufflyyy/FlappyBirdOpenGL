#pragma once

typedef struct {
	unsigned int id;

    int width;
    int height;
} Texture;

Texture createTexture(const char* filePath);
void useTexture(Texture texture);