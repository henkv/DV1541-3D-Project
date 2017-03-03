#version 440 core

layout(location = 0) in vec3 FragPosition;
layout(location = 1) in vec3 FragNormal;
layout(location = 2) in vec3 FragColor;

layout(location = 0) out vec4 Color;

void main()
{
	
	Color = vec4((FragNormal+vec3(1))/2, 1.0);
}