#version 440 core

// IN //
layout(location = 0) in vec2 FragTexCoord;

// OUT //
layout(location = 0) out vec4 Color;

// UNIFORM //
layout(location = 0) uniform sampler2D Positions;
layout(location = 1) uniform sampler2D Normals;
layout(location = 2) uniform sampler2D ColorSpecs;
layout(location = 3) uniform vec3 viewPosition;

struct Light
{
	vec3 position;
	vec3 color;
};

layout (std140, binding = 0) uniform Lights
{
	Light lights[100];
	int nrOfLights;
};


vec3 diffuseColor(Light light, vec3 fragPosition, vec3 fragNormal)
{
	vec3 lightDir = normalize(light.position.xyz - fragPosition);
	float factor = max(dot(fragNormal, lightDir), 0);

	return light.color.rgb * factor;
}

vec3 specularColor(Light light,
				   vec3 fragPosition, vec3 fragNormal,
				   vec3 viewPosition, float specularStrength)
{
	vec3 lightDir = normalize(light.position.xyz - fragPosition);
	vec3 viewDir = normalize(viewPosition - fragPosition);
	vec3 reflectDir = reflect(-lightDir, fragNormal);
	float factor = pow(max(dot(viewDir, reflectDir), 0), 4);

	return light.color.rgb * factor * 1;
}

float calcAttenuation(float dist, float constant, float linear, float quadratic)
{
	return (1.0f / (constant + linear * dist + quadratic * (dist * dist)));
}

void main()
{
	float constant	= 1.0f;
	float linear		= 0.045f;
	float quadratic	= 0.0075f;

	vec3 ambientColor = vec3(1, 1, 1);
	float ambientStrength = 0.1;

	vec3 position = texture(Positions, FragTexCoord).xyz;
	vec3 normal = texture(Normals, FragTexCoord).xyz;
	vec4 colorSpec = texture(ColorSpecs, FragTexCoord);
	vec3 color = colorSpec.rgb;
	float specularStrength = colorSpec.a;




	vec3 lightSum = ambientColor * ambientStrength;
	for(int i = 0; i < nrOfLights; i++)
	{
		float dist = length(lights[i].position - position);
		float attenuation = calcAttenuation(dist, constant, linear, quadratic);

		lightSum += (
			diffuseColor(lights[i], position, normal) * attenuation +
			specularColor(lights[i], position, normal, viewPosition, specularStrength) * attenuation
		);
	}


	Color = vec4(lightSum * color, 1);

}
