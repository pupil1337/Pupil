#version 330 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 SecondColor;

in vec4 Color;
in vec2 TexCoord;
in float TexIndex;
in float TilingFactor;

uniform sampler2D Textures[32];

void main() {

	FragColor = texture(Textures[int(TexIndex)], TexCoord * TilingFactor) * Color;
	SecondColor = vec4(0.2f, 0.3f, 0.3f, 1.0f);
}