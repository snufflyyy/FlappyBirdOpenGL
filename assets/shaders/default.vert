#version 330 core

layout (location = 0) in vec4 aPosTex;

out vec2 textureCoord;

uniform mat4 projection;
uniform mat4 transform;

void main() {
	gl_Position = projection * transform * vec4(aPosTex.x, aPosTex.y, 0.0f, 1.0f);

	textureCoord = vec2(aPosTex.z, aPosTex.w);
}