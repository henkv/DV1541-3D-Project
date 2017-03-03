#pragma once
#include <vector>
#include <glm\glm.hpp>
using namespace std;
using namespace glm;

class Mesh
{
public:
	static const size_t VertexSize;
	static const void * PositionOffset;
	static const void * TexCoordOffset;
	static const void * NormalOffset;

private:
	struct Vertex
	{
		vec3 position;
		vec2 texCoord;
		vec3 normal;
	};

	vector<Vertex> vertices;

public:
	Mesh();
	~Mesh();

	void parseObj(const char * objPath);

	const size_t size() const;
	const size_t bufferSize() const;
	const void * bufferData() const;
};

