#version 440 core

layout(location = 0) in vec3 FragPosition;
layout(location = 1) in vec3 FragNormal;
layout(location = 2) in vec3 FragColor;

layout(location = 0) out vec3 Position;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec4 ColorSpec;

void main()
{
	Position = FragPosition;
	Normal = FragNormal;
	ColorSpec = vec4(0.2, 0.6, 1.0, 1.0);
}