#include "sprite.h"

#include <glad/glad.h>
#include <cglm/cglm.h>

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "render.h"

Sprite createSprite(Shader shader, Texture texture) {
	Sprite sprite = {0};

    sprite.shader = shader;
    sprite.texture = texture;

	// default scale
    sprite.scale[0] = 100;
    sprite.scale[1] = 100;

	// default position
    sprite.position[0] = (float) windowWidth / 2;
    sprite.position[1] = (float) windowHeight / 2;

	// init transform matrix
	glm_mat4_identity(sprite.transform);

	// set projection
	useShader(sprite.shader);
	glUniformMatrix4fv(glGetUniformLocation(sprite.shader.id, "projection"), 1, GL_FALSE, (float*) projection);
	glUseProgram(0);

	return sprite;
}

// very basic aabb collision, thanks learnopengl.com! 
bool getSpriteCollision(Sprite *spriteOne, Sprite *spriteTwo) {
    bool collisionX = spriteOne->position[0] - spriteOne->scale[0] / 2 + spriteOne->scale[0] >= spriteTwo->position[0] - spriteTwo->scale[0] / 2 &&
                      spriteTwo->position[0] - spriteTwo->scale[0] / 2 + spriteTwo->scale[0] >= spriteOne->position[0] - spriteOne->scale[0] / 2;

    bool collisionY = spriteOne->position[1] - spriteOne->scale[1] / 2 + spriteOne->scale[1] >= spriteTwo->position[1] - spriteTwo->scale[1] / 2 &&
                      spriteTwo->position[1] - spriteTwo->scale[1] / 2 + spriteTwo->scale[1] >= spriteOne->position[1] - spriteOne->scale[1] / 2;

    return collisionX && collisionY;
}

void renderSprite(Sprite sprite) {
	// set rotation
	glm_rotated(sprite.transform, glm_rad(sprite.rotation), (vec3) {0.0f, 0.0f, 1.0f});
	// set position
	glm_translated(sprite.transform, (vec3) {sprite.position[0], sprite.position[1], 0.0f});
	// set scale
	glm_scale(sprite.transform, (vec3) {sprite.scale[0], sprite.scale[1], 0.0f});

	useShader(sprite.shader);

	// set transform
	glUniformMatrix4fv(glGetUniformLocation(sprite.shader.id, "transform"), 1, GL_FALSE, (float*) sprite.transform);

	useTexture(sprite.texture);
	
	// draw it
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void deleteSprite(Sprite *sprite) {
	deleteShader(&sprite->shader);
	deleteTexture(&sprite->texture);
}

