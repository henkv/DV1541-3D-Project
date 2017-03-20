#include "Vertex.h"

const unsigned int Vertex::VertexSize = sizeof(Vertex);
const void * Vertex::PositionOffset = (void *)offsetof(Vertex, position);
const void * Vertex::TexCoordOffset = (void *)offsetof(Vertex, texCoord);
const void * Vertex::NormalOffset = (void *)offsetof(Vertex, normal);

Vertex::Vertex(vec3 position, vec2 texCoord, vec3 normal)
	: position(position)
	, texCoord(texCoord)
	, normal(normal) {}