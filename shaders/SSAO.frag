#version 440 core

layout(location = 0) in vec2 TexCoord;
layout(location = 0) out vec4 AmbientFactor;

uniform sampler2D viewPositions;
uniform mat4 projectionMatrix;

const int MAX_RANDOM_POINTS = 128;
uniform vec3 randomPoints[MAX_RANDOM_POINTS];
uniform int nrOfRandomPoints;

void main()
{
	float sampleRadius = 0.1;

	vec3 ambientPoint = vec3(texture(viewPositions, TexCoord));
	float ambientFactor = 0.0f;

	for (int i = 0; i < nrOfRandomPoints; i++)
	{
		vec3 randomPoint = ambientPoint + randomPoints[i];
		vec4 offset = projectionMatrix * vec4(randomPoint, 1);
		vec2 offsetUV = (offset.xy / offset.w) * 0.5 + 0.5;

		float sampleDepth = texture(viewPositions, offsetUV).z;

		if (abs(ambientPoint.z - sampleDepth) < sampleRadius)
		{
				ambientFactor += step(sampleDepth, randomPoint.z);
		}
	}

	ambientFactor = 1.0 - ambientFactor / nrOfRandomPoints;
	AmbientFactor = vec4(pow(ambientFactor, 2));
}
