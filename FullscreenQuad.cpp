#include "FullscreenQuad.h"


FullscreenQuad::FullscreenQuad()
{
	Vertex vertices[4] = {
		{ { -1, -1, 0 }, { 0, 0 } },
		{ { -1,  1, 0 }, { 0, 1 } },
		{ {  1, -1, 0 }, { 1, 0 } },
		{ {  1,  1, 0 }, { 1, 1 } }
	};

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &arrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));

}

FullscreenQuad::~FullscreenQuad()
{
}

void FullscreenQuad::draw()
{
	glBindVertexArray(vertexArray);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
