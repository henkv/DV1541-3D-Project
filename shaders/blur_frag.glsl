#version 440 core

out vec4 FragColor;
layout(location = 0)in vec2 TexCoords;

uniform sampler2D image;
uniform bool horizontal;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	vec2 tex_offset = 1.0 / textureSize(image, 0);
	vec3 result = texture(image, TexCoords).rgb * weight[0];
	if (horizontal)
	{
		for(int i= 0; i < 5; ++i)
		{
			result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
			result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
		}
	}
	else
	{
		for(int i= 0; i < 5; ++i)
		{
			result += texture(image, TexCoords + vec2(tex_offset.y * i, 0.0)).rgb * weight[i];
			result += texture(image, TexCoords - vec2(tex_offset.y * i, 0.0)).rgb * weight[i];
		}
	}

	FragColor = vec4(result, 1.0);

}