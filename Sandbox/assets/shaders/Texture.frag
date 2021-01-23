#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec4 Color;
uniform sampler2D Texture0;

void main() {
	FragColor = texture(Texture0, TexCoord * 10.0f) * Color;
}