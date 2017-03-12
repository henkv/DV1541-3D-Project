#include "FullscreenQuad.h"


FullscreenQuad::FullscreenQuad()
	: textureShader("shaders/FullscreenQuad.vert", "shaders/FullscreenTexture.frag")
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

void FullscreenQuad::drawTexture(GLuint texture)
{
	textureShader.use();
	textureShader.setTexture2D(0, "fullscreenTexture", texture);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw();
}
