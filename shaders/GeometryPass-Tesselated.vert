#version 440 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTexCoord;
layout(location = 2) in vec3 VertexNormal;

layout(location = 0) out vec3 WorldPosition;
layout(location = 1) out vec2 TexCoord;
layout(location = 2) out vec3 Normal;
//layout(location = 3) out vec3 ViewPosition;


uniform mat4 worldMatrix;

void main()
{
	WorldPosition = vec3(worldMatrix * vec4(VertexPosition, 1));
	//ViewPosition = vec3(viewMatrix * vec4(WorldPosition, 1));
	TexCoord = VertexTexCoord;
	Normal = vec3(worldMatrix * vec4(VertexNormal, 0));
}
