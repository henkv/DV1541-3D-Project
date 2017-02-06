#version 440 core
layout(location = 0) in vec3 fragment_position;
layout(location = 1) in vec3 fragment_normal;

layout(location = 3) uniform vec3 viewPosition;

out vec4 fragment_color;
out vec4 brightColor;
	
vec3 ambient_light = vec3(0.f, 0.f, 0.f);
vec3 light_position = vec3(0.f, 0.f, -100.f);
vec3 light_color = vec3(1.f, 1.f, 1.f);
float specularStrength = 0.5f;

vec4 ambient()
{
  return vec4(ambient_light, 1.f);
}

vec4 diffuse()
{
  vec4 diffuseColor = vec4(light_color, 1.f);
  vec3 lightDir = normalize(light_position - fragment_position);


  return diffuseColor * max(dot(fragment_normal, lightDir), 0.f);
}

vec4 specular()
{
 	vec4 specularColor = vec4(light_color, 1.f);

	vec3 lightDir = normalize(light_position - fragment_position);
	vec3 viewDir = normalize(viewPosition - fragment_position);
	vec3 reflectDir = reflect(-lightDir, fragment_normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);

	return specularColor * spec;
}

void main()
{
  fragment_color = normalize(gl_FragCoord);
  fragment_color *= (ambient() + diffuse() + specular());
}
