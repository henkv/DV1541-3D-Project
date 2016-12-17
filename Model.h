#pragma once
#include <glad\glad.h>
#include "Mesh.h"

class Model
{
private:
	Mesh mesh;

	GLuint vertexArray;
	GLuint vertexBuffer;
	GLsizei vertexArrayLength;	

public:
	Model(const char * objPath);
	~Model();

	void draw() const;
};

