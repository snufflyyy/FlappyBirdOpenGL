#include "sprite.h"

#include <glad/glad.h>
#include <cglm/cglm.h>

#include "cglm/affine-post.h"
#include "cglm/affine.h"
#include "cglm/types.h"
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

Sprite createSprite(vec2 position, vec2 scale, Shader shader, Texture texture) {
	Sprite sprite = {0};

	// set position
	sprite.position[0] = position[0];
	sprite.position[1] = position[1];

	// set scale
	sprite.scale[0] = scale[0];
	sprite.scale[1] = scale[1];

	sprite.shader = shader;
	sprite.texture = texture;

	// init transform matrix
	glm_mat4_identity(sprite.transform);

	// set projection
	useShader(sprite.shader);
	glUniformMatrix4fv(glGetUniformLocation(sprite.shader.id, "projection"), 1, GL_FALSE, (float*) projection);
	glUseProgram(0);

	return sprite;
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

