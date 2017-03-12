#version 440 core
layout(location = 0) in vec2 TexCoords;

layout(location = 0) out vec4 Color;

uniform sampler2D scene;
uniform sampler2D glow;

void main()
{
	const float gamma = 3;
	const float exposure = 1;

	vec3 sceneColor = texture(scene, TexCoords).rgb;
	vec3 glowColor = texture(glow, TexCoords).rgb;

	vec3 result = vec3(1.0) - exp(-(sceneColor + glowColor) * exposure);	//tonemapping
	result = pow(result, vec3(1.0 / gamma));	//gamma correct

	Color = vec4(result, 1.0f);
}
