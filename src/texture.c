#include "texture.h"

#include <stdbool.h>

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static unsigned char* loadImage(const char* path, int* width, int* height, int* channels) {
	// makes the image not upside down
	stbi_set_flip_vertically_on_load(true);

	// loads that image up
	unsigned char* image = stbi_load(path, width, height, channels, 4);
	if (!image) {
		printf("Failed to load Image!\n");
	}

	return image;
}

Texture createTexture(const char* filePath) {
	Texture texture = {0};

	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, channels;
	unsigned char* image = loadImage(filePath, &width, &height, &channels);

	// this assumes all images used are pngs
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// gone so soon :(
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

void useTexture(Texture texture) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.id);
}