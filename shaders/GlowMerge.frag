#version 440 core
layout(location = 0) in vec2 TexCoords;

layout(location = 0) out vec4 Color;

uniform sampler2D scene;
uniform sampler2D glow;

void main()
{
	const float gamma = 1.5f;
	const float exposure = 3.0f;

	vec3 sceneColor = texture(scene, TexCoords).rgb;
	vec3 glowColor = texture(glow, TexCoords).rgb;

	vec3 result = vec3(1.0) - exp(-(sceneColor + glowColor * 0.25f) * exposure);	//tonemapping
	result = pow(result, vec3(1.0 / gamma));	//gamma correct

	Color = vec4(result, 1.0f);
}
