#version 440 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture(scene, TexCoords).rgb;
	vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;

	if(bloom)
	{
		hdrColor += bloomColor;   // add. blending
	}

	//tonemapping
	vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
	//gamma correct
	//result = pow(result, vec3 (1.0/gamma));


	FragColor = vec4(result, 1.0f);
}