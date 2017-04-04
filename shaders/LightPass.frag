#version 440 core

// IN //
layout(location = 0) in vec2 FragTexCoord;

// OUT //
layout(location = 0) out vec4 Color;

// UNIFORM //
uniform sampler2D Positions;
uniform sampler2D Normals;
uniform sampler2D ColorSpecs;
uniform sampler2D SSAOMap;

uniform vec3 viewPosition;

uniform vec3 sunColor;
uniform vec3 sunDirection;
uniform mat4 sunLightSpaceMatrix;
uniform sampler2D sunShadowMap;

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

vec3 getPointLightSum(vec3 fragPosition, vec3 fragNormal, float specularStrength, float ao)
{
	float constant	= 1.0f;
	float linear		= 0.045f;
	float quadratic	= 0.0075f;

	vec3 ambientColor = vec3(1, 1, 1);
	float ambientStrength = 0.1;

	vec3 lightSum = ambientColor * ambientStrength * ao;
	for(int i = 0; i < nrOfLights; i++)
	{
		float dist = length(lights[i].position - fragPosition);
		float attenuation = calcAttenuation(dist, constant, linear, quadratic);

		lightSum += (
			diffuseColor(lights[i], fragPosition, fragNormal) * attenuation +
			specularColor(lights[i], fragPosition, fragNormal, viewPosition, specularStrength) * attenuation
		);
	}

	return lightSum;
}

float getShadowFactor(vec3 fragPosition, vec3 fragNormal, vec3 lightDir, mat4 lightSpaceMatrix, sampler2D shadowMap)
{
	vec4 lightSpacePosition = lightSpaceMatrix * vec4(fragPosition, 1);
	vec3 uvz = (lightSpacePosition.xyz / lightSpacePosition.w) * 0.5 + 0.5;

	float closestDepth = texture(shadowMap, uvz.xy).r;
	float currentDepth = uvz.z;

	float bias = 0.005;// max(0.05 * (1.0 - dot(fragNormal, lightDir)), 0.005);

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
			for(int y = -1; y <= 1; ++y)
			{
					float pcfDepth = texture(shadowMap, uvz.xy + vec2(x, y) * texelSize).r;
					shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
			}
	}
	shadow /= 9.0;

	if(currentDepth > 1.0)
	{
	   shadow = 0.0;
	}

	return 1 - shadow;
}

vec3 getSunLight(vec3 fragPosition, vec3 fragNormal, float ao)
{
	float ambientFactor = 0.2 * ao;
	float shadowFactor = getShadowFactor(fragPosition, fragNormal, -sunDirection, sunLightSpaceMatrix, sunShadowMap);
	float diffuseFactor = max(dot(fragNormal, -sunDirection), 0.0);

	return sunColor * (ambientFactor + shadowFactor * diffuseFactor);
}

void main()
{
	vec3 position = texture(Positions, FragTexCoord).xyz;
	vec3 normal = texture(Normals, FragTexCoord).xyz;
	vec4 colorSpec = texture(ColorSpecs, FragTexCoord);
	vec3 color = colorSpec.rgb;
	float specularStrength = colorSpec.a;

	float ambientFactor = texture(SSAOMap, FragTexCoord).x;

	vec3 lightSum = getPointLightSum(position, normal, specularStrength, ambientFactor);
	lightSum += getSunLight(position, normal, ambientFactor);


	Color = vec4(lightSum, 1);

}
