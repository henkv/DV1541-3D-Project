#version 440 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec3 vertex_color;

layout(location = 0) out vec3 position;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec3 fcolor;
layout(location = 3) out vec4 fragPosLightSpace;

layout(location = 2) uniform mat4 projection;
layout(location = 1) uniform mat4 view;
layout(location = 0) uniform mat4 stillWorld;
layout(location = 3) uniform mat4 lightSpaceMatrix;

void main()
{
	fcolor = vertex_color;
	position = vec3(stillWorld * vec4(vertex_position, 1.0f));
	normal = normalize(vec3(stillWorld * vec4(vertex_normal, 0.0f)));
	fragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0f) ;

	gl_Position = projection * view * vec4(position, 1.0f) ;
}

//#version 330 core
//layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 normal;
//layout (location = 2) in vec2 texCoords;
//
//out VS_OUT {
//    vec3 FragPos;
//    vec3 Normal;
//    vec2 TexCoords;
//    vec4 FragPosLightSpace;
//} vs_out;
//
//uniform mat4 projection;
//uniform mat4 view;
//uniform mat4 model;
//uniform mat4 lightSpaceMatrix;
//
//void main()
//{
//    gl_Position = projection * view * model * vec4(position, 1.0f);
//    vs_out.FragPos = vec3(model * vec4(position, 1.0));
//    vs_out.Normal = transpose(inverse(mat3(model))) * normal;
//    vs_out.TexCoords = texCoords;
//    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
//}