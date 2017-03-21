#version 440 core

layout(location = 0) in vec3 FragmentWorldPosition;
layout(location = 1) in vec2 FragmentTexCoord;
layout(location = 2) in vec3 FragmentNormal;
layout(location = 3) in vec3 FragmentViewPosition;

layout(location = 0) out vec3 Position;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec4 ColorSpec;
layout(location = 3) out vec3 ViewPosition;

uniform sampler2D diffuseMap;

void main()
{
	Position = FragmentWorldPosition;
	ViewPosition = FragmentViewPosition;
	Normal = FragmentNormal;
	ColorSpec = vec4(texture(diffuseMap, FragmentTexCoord).rgb, 1);
}
