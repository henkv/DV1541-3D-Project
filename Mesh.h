#pragma once
#include <vector>
#include <glm\glm.hpp>
using namespace std;
using namespace glm;

class Mesh
{
private:
	struct Vertex
	{
		vec3 position;
		vec3 normal;
	};

	vector<Vertex> vertices;
public:
	static const GLsizei VertexSize;
	static const GLvoid * PositionOffset;
	static const GLvoid * NormalOffset;

public:
	Mesh();
	~Mesh();

	void parseObj(const char * objPath);

	GLsizeiptr bufferSize() const;
	const GLvoid * bufferData() const;
	GLsizei size() const;
};

