#include "Model.h"
#include <fstream>
#include <sstream>
#include <string>


Model::Model(const char * objPath)
{
	mesh.parseObj(objPath);
	vertexArrayLength = mesh.size();

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.bufferSize(), mesh.bufferData(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Mesh::VertexSize, Mesh::PositionOffset);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Mesh::VertexSize, Mesh::TexCoordOffset);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, Mesh::VertexSize, Mesh::NormalOffset);

	glBindVertexArray(0);

}

Model::~Model()
{
}

void Model::move(vec3 delta)
{
	position += delta;
	worldMatrix = translate(worldMatrix, delta);
}

void Model::setPosition(vec3 position)
{
	this->position = position;
	worldMatrix = translate(mat4(), position);
}

void Model::draw(Shader & shader)
{
	shader.setUniform("worldMatrix", worldMatrix);
	glBindVertexArray(vertexArray);
	glDrawArrays(GL_TRIANGLES, 0, vertexArrayLength);
}

void Model::update(float delta)
{
}
