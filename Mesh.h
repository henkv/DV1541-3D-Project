#pragma once
#include <glad\glad.h>
#include <vector>
#include <glm\glm.hpp>
#include "Vertex.h"
class Mesh
{
private:
	std::vector<Vertex> vertices;

	GLuint vertexArray;
	GLuint vertexBuffer;
	GLsizei vertexArrayLength;

	const size_t size() const;
	const size_t bufferSize() const;
	const void * bufferData() const;

public:
	Mesh(std::vector<Vertex> & vertices = std::vector<Vertex>());
	~Mesh();

	void draw();
	void drawTesselated();
};

