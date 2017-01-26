#version 440 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

layout(location = 0) out vec3 position;
layout(location = 1) out vec3 normal;

layout(location = 0) uniform mat4 world;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 projection;
layout(location = 3) uniform vec3 viewPosition;

void main()
{
  position = vec3(world * vec4(vertex_position, 1.0f));
  normal = normalize(vec3(world * vec4(vertex_normal, 0.0f)));

  gl_Position = projection * view * vec4(position, 1.0f);
}
