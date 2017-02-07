#pragma once
#include <glad\glad.h>
#include <glm\gtc\matrix_transform.hpp>
#include "Mesh.h"
#include "GameObject.h"

class Model : public GameObject
{
private:
	Mesh mesh;

	GLuint vertexArray;
	GLuint vertexBuffer;
	GLsizei vertexArrayLength;

	mat4 worldMatrix;
	vec3 position;

public:
	Model(const char * objPath);
	~Model();

	void move(vec3 delta);
	void setPosition(vec3 position);

	// Inherited via GameObject
	virtual void draw(Shader & shader) override;
	virtual void update(float delta) override;
};

