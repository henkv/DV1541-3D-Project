#version 440 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexUV;

layout(location = 0) out vec2 UV;

void main()
{
	UV = VertexUV;
	
	gl_Position = vec4(VertexPosition, 1);
}