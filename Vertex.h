#pragma once
#include <glm\glm.hpp>
using namespace glm;

class Vertex
{
public:
	vec3 position;
	vec2 texCoord;
	vec3 normal;

	Vertex(vec3 position, vec2 texCoord = vec2(0, 0), vec3 normal = vec3(0, 0, 0));

	static const unsigned int VertexSize;
	static const void * PositionOffset;
	static const void * TexCoordOffset;
	static const void * NormalOffset;
};

