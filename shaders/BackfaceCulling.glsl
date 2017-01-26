#version 440 core

layout (triangle) in;
layout (triangle_strip, max_vertices = 3) out;

layout(location = 0) in vec3 position[];
layout(location = 1) in vec3 normal[];

layout(location = 0) uniform mat4 world;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 projection;
layout(location = 3) uniform float globalTime;
layout(location = 4) uniform vec3 viewPoint;


layout(location = 0) out vec3 fragment_position;
layout(location = 1) out vec3 fragment_normal;



vec3 getTriangleNormal(vec3 p0, vec3 p1, vec3 p2)
{
  vec3 normal;

  vec3 U = p1 - p0;
  vec3 V = p2 - p0;

  normal = cross(U,V);

  return normalize(normal);
}

void main()
{
	vec3 normal = getTriangleNormal(position[0], position[1], position[2]);

	vec3 transformed_normal = (vec4(normal, 0.0f) * world).xyz;
	vec3 vt = normalize (position[0].gl_Position.xyz - viewPoint);

	float d = dot(vt, normal);

	if (normal > 0)
	{
		fragment_position = normal;

		EmitVertex();
	}

	EndPrimitive();

	return 0;
}