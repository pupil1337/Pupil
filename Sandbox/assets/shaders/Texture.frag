#version 330 core
out vec4 FragColor;

//in vec2 TexCoord;
in vec4 Color;

//uniform vec4 Color;
//uniform sampler2D Texture0;
//uniform float FilingFactor;

void main() {
	//FragColor = texture(Texture0, TexCoord * FilingFactor) * Color;
	FragColor = Color;
}