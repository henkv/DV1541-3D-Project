#version 440 core
layout (vertices = 3) out;

layout(location = 0) in vec3 VertexWorldPosition[];
layout(location = 1) in vec2 VertexTexCoord[];
layout(location = 2) in vec3 VertexNormal[];

layout(location = 0) out vec3 WorldPosition[];
layout(location = 1) out vec2 TexCoord[];
layout(location = 2) out vec3 Normal[];

uniform vec3 eyePosition;

float getTesselationFactor(float distance1, float distance2)
{
	float averageDistance = (distance1 + distance2) / 2.0;
	float factor = 3.0;

	if (averageDistance <= 2.0)
	{
		factor = 10.0;
	}
	else if (averageDistance <= 5)
	{
		factor = 7.0;
	}

	return factor;
}

void main()
{
	WorldPosition[gl_InvocationID] = VertexWorldPosition[gl_InvocationID];
	TexCoord[gl_InvocationID] = VertexTexCoord[gl_InvocationID];
	Normal[gl_InvocationID] = VertexNormal[gl_InvocationID];

	float vertexEyeDistance0 = distance(eyePosition, WorldPosition[0]);
	float vertexEyeDistance1 = distance(eyePosition, WorldPosition[1]);
	float vertexEyeDistance2 = distance(eyePosition, WorldPosition[2]);

	gl_TessLevelOuter[0] = getTesselationFactor(vertexEyeDistance1, vertexEyeDistance2);
	gl_TessLevelOuter[1] = getTesselationFactor(vertexEyeDistance2, vertexEyeDistance0);
	gl_TessLevelOuter[2] = getTesselationFactor(vertexEyeDistance0, vertexEyeDistance1);
	gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}
