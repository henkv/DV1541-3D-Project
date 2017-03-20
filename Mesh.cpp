#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <glad\glad.h>


const size_t Mesh::size() const
{
	return vertices.size();
}

const size_t Mesh::bufferSize() const
{
	return Vertex::VertexSize * vertices.size();
}

const void * Mesh::bufferData() const
{
	return &vertices[0];
}

void Mesh::draw()
{
	glBindVertexArray(vertexArray);
	glDrawArrays(GL_TRIANGLES, 0, vertexArrayLength);
}

Mesh::Mesh(std::vector<Vertex>& vertices)
	: vertices(vertices) 
{
	vertexArrayLength = vertices.size();

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, bufferSize(), bufferData(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::VertexSize, Vertex::PositionOffset);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Vertex::VertexSize, Vertex::TexCoordOffset);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, Vertex::VertexSize, Vertex::NormalOffset);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
}
