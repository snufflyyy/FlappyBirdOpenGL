#version 330 core

out vec4 FragColor;

in vec2 textureCoord;

uniform sampler2D inputTexture;

void main() {
	// this is done to allow transparency
	vec4 textureColor = texture(inputTexture, textureCoord);
	if (textureColor.a < 0.1f) {
		discard;
	}

	FragColor = textureColor;
}