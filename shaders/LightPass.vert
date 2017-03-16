#version 440 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTexCoord;

layout(location = 0) out vec2 TexCoord;
layout(location = 1) out vec3 ligtSpacePosition;

void main()
{
	gl_Position = vec4(VertexPosition, 1);
	TexCoord = VertexTexCoord;
}
