#version 440 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec3 VertexColor;

layout(location = 0) out vec3 Position;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec3 Color;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	Position = (worldMatrix * vec4(VertexPosition, 1)).xyz;
	Normal = (worldMatrix * vec4(VertexNormal, 0)).xyz;
	Color = VertexColor;

	gl_Position = projectionMatrix * viewMatrix * vec4(Position, 1);
}