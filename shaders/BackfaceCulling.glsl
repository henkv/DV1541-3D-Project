#version 440 core

layout (triangles) in;
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

void main()
{
	vec3 vt = normalize(viewPoint - position[0]);

	float d = dot(vt, normal[0]);

	if (d > 0)
	{
		for (int i = 0; i < 3; i++)
		{
			gl_Position = projection * view * vec4(position[i], 1);
			fragment_position = position[i];
			fragment_normal = normal[i];
			EmitVertex();
		}
	}
		
	
	EndPrimitive();

}