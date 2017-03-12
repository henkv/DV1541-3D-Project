#version 440 core

layout(location = 0) in vec2 FragmentTexCoord;

layout(location = 0) out vec4 Color;

uniform sampler2D fullscreenTexture;

void main()
{
	vec3 result = texture(fullscreenTexture, FragmentTexCoord).rgb;
	Color = vec4(result, 1);
}
