#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
			
uniform mat4 ProjectionView;
//uniform mat4 Model;

//out vec2 TexCoord;
out vec4 Color;

void main() {
	//TexCoord = aTexCoord;
	Color = aColor;
	gl_Position = ProjectionView  * vec4(aPos, 1.0f);
}