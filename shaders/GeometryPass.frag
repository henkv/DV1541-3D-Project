#version 440 core

layout(location = 0) in vec3 FragmentPosition;
layout(location = 1) in vec2 FragmentTexCoord;
layout(location = 2) in vec3 FragmentNormal;

layout(location = 0) out vec3 Position;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec4 ColorSpec;

void main()
{
	Position = FragmentPosition;
	Normal = FragmentNormal;

	float x = length(Position);
	ColorSpec = vec4(1,1,1,1);
}
