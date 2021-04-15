#version 330 core
layout(location = 0) in	float aEntityID;
layout(location = 1) in vec3 aPos;
layout(location = 2) in vec4 aColor;
layout(location = 3) in vec2 aTexCoord;
layout(location = 4) in float aTexIndex;
layout(location = 5) in float aTilingFactor;

uniform mat4 ProjectionView;
//uniform mat4 Model;

out float EntityID;
out vec4 Color;
out vec2 TexCoord;
out float TexIndex;
out float TilingFactor;

void main() {

	EntityID = aEntityID;
	Color = aColor;
	TexCoord = aTexCoord;
	TexIndex = aTexIndex;
	TilingFactor = aTilingFactor;

	gl_Position = ProjectionView  * vec4(aPos, 1.0f);
}