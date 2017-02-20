#version 440 core

layout (location = 0) out vec4 fragment_color;
layout (location = 1) out vec4 bright_color;

in vec3 fragment_position;
in vec3 fragment_normal;
in vec2 tex_coords;

uniform vec3 lightColor;

void main()
{
	fragment_color = vec4(lightColor, 1.0);
	float brightness= dot(fragment_color.rgb, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 1.0)
	{
		bright_color = vec4(fragment_color.rgb, 1.0);
	}
}