#version 44 core

layout (triangle) in;
layout (triangle_strip, max_vertices = 3) out;

layout(location = 0) in vec3 position[];
layout(location = 1) in vec3 normal[];




layout(location = 0) out vec3 fragment_position;
layout(location = 1) out vec3 fragment_normal;



vec3 getTriangleNormal(vec4 p0, vec4 p1, vec4 p2)
{
  vec3 normal;

  vec3 U = p1.xyz - p0.xyz;
  vec3 V = p2.xyz - p0.xyz;

  normal = cross(U,V);

  return normalize(normal);
}

void main()
{
	vec3 normal = getTriangleNormal(position[0], position[1], position[2]);

	if (normal > 0)
	{
		fragment_position = normal;

		EmitVertex();
	}

	EndPrimitive();

	return 0;
}