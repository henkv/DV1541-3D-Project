#version 440 core

layout(location = 0) in vec3 FragmentPosition;
layout(location = 1) in vec2 FragmentTexCoord;
layout(location = 2) in vec3 FragmentNormal;

layout(location = 0) out vec4 Glow;

uniform sampler2D diffuseMap;

void main()
{
	vec4 color = texture(diffuseMap, FragmentTexCoord);
	Glow = vec4(color * color.a);
}
