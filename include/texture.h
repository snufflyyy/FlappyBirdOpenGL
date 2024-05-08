#pragma once

typedef struct {
	unsigned int id;
} Texture;

Texture createTexture(const char* filePath);
void useTexture(Texture texture);