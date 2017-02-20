#version 440 core

layout(location = 0) in vec2 FragUV;

layout(location = 0) out vec4 Color;

layout(location = 0)uniform sampler2D Positions;
layout(location = 1)uniform sampler2D Normals;
layout(location = 2)uniform sampler2D ColorSpecs;

uniform vec3 viewPosition;

vec3 ambientColor(vec3 lightColor, float ambientStrength)
{
	return lightColor * ambientStrength;
}

vec3 diffuseColor(vec3 lightColor, vec3 lightPosition, vec3 fragPosition, vec3 fragNormal)
{
	vec3 lightDir = normalize(lightPosition - fragPosition);
	float factor = max(dot(fragNormal, lightDir), 0);

	return lightColor * factor;
}

vec3 specularColor(vec3 lightColor, vec3 lightPosition, 
				   vec3 fragPosition, vec3 fragNormal, 
				   vec3 viewPosition, float specularStrength)
{
	vec3 lightDir = normalize(lightPosition - fragPosition);
	vec3 viewDir = normalize(viewPosition - fragPosition);
	vec3 reflectDir = reflect(-lightDir, fragNormal);
	float factor = pow(max(dot(viewDir, reflectDir), 0), 4);

	return lightColor * factor * 1;
}

void main()
{
	vec3 lightColor = vec3(1, 1, 1);
	vec3 lightPosition = vec3(0, 0, -200);
	float ambientStrength = 0.1;

	vec3 position = texture(Positions, FragUV).xyz;
	vec3 normal = texture(Normals, FragUV).xyz;
	vec4 colorSpec = texture(ColorSpecs, FragUV);
	vec3 color = colorSpec.rgb;
	float specularStrength = colorSpec.a;

	Color = vec4((
		ambientColor(lightColor, ambientStrength) +
		diffuseColor(lightColor, lightPosition, position, normal) +
		specularColor(lightColor, lightPosition, position, normal, viewPosition, specularStrength)
	) * color, 1);
}