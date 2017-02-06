#version 440 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec4 out_color;

uniform bool horizontal;

//Värden från en kernel calculator som räknar ut hur stor påverkan
//varje omgivande pixel borde ha.
uniform float weight[5] = float[] (0.023792, 0.094907, 0.150342, 0.094907, 0.023792)

void main()
{
	vec 4 color = in_color;
	float brightness = color.r + color.g + color.b) / 3.0;
	color = in_color * brightness;

	vec2 offset = 1.0;	//size of a pixel
	vec3 result = in_color.rgb * weight[0];

	if(horizontal)
	{
		for(int i = 1; i < 5; ++i)
		{
			result += position + (offset.x * i, 0.0)) * weight[i];
			result += position - (offset.x * i, 0.0)) * weight[i];
		}
	}
	else
	{
		for(int i = 1; i < 5; ++i)
		{
			result += position + (offset.y * i, 0.0)) * weight[i];
			result += position - (offset.y * i, 0.0)) * weight[i];
		}
	}

	
	out_color = vec4(result, 1.0);
}