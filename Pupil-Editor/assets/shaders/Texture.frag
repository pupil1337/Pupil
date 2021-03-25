#version 330 core
out vec4 FragColor;

in vec4 Color;
in vec2 TexCoord;
in float TexIndex;
in float TilingFactor;

uniform sampler2D Textures[32];

void main() {

	FragColor = texture(Textures[int(TexIndex)], TexCoord * TilingFactor) * Color;
}