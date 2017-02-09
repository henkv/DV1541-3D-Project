#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>
#include "GameObject.h"

using namespace glm;

class FullscreenQuad : public GameObject
{
private:
	struct Vertex
	{
		vec3 position;
		vec2 uv;
	};

	GLuint vertexArray;
	GLuint arrayBuffer;

public:
	FullscreenQuad();
	~FullscreenQuad();

	// Inherited via GameObject
	virtual void draw(Shader & shader) override;
	virtual void update(float delta) override;
};

