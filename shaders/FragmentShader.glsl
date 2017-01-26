#version 440 core
layout(location = 0) in vec3 fragment_position;
layout(location = 1) in vec3 fragment_normal;

out vec4 fragment_color;

vec3 ambient_light = vec3(0.f, 0.f, 0.f);
vec3 light_position = vec3(0.f, 0.f, -100.f);
vec3 light_color = vec3(1.f, 1.f, 1.f);

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

void main()
{
  fragment_color = vec4(abs(normalize(fragment_position)), 1.f);
  fragment_color *= (ambient() + diffuse());
}
