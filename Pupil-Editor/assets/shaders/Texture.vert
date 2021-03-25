#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aTexIndex;
layout(location = 4) in float aTilingFactor;
			
uniform mat4 ProjectionView;
//uniform mat4 Model;

out vec4 Color;
out vec2 TexCoord;
out float TexIndex;
out float TilingFactor;

void main() {

	Color = aColor;
	TexCoord = aTexCoord;
	TexIndex = aTexIndex;
	TilingFactor = aTilingFactor;

	gl_Position = ProjectionView  * vec4(aPos, 1.0f);
}