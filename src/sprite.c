#include "sprite.h"

#include <glad/glad.h>
#include <cglm/cglm.h>

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "render.h"

const float vertices[16] = {
	// position			   // texture coords	
     0.5f,  0.5f,  1.0f, 1.0f,  // top right
     0.5f, -0.5f,  1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
   -0.5f, 0.5f, 0.0f, 1.0f  // top left 
};
const unsigned int indices[6] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  

Sprite createSprite(Shader shader, Texture texture) {
	Sprite sprite = {0};

    sprite.shader = shader;
    sprite.texture = texture;

    sprite.scale[0] = 100;
    sprite.scale[1] = 100;

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

bool getCollision(Sprite *spriteOne, Sprite *spriteTwo) {
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

	// unbind all the shit (not doing this makes it way faster)
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glUseProgram(0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}

