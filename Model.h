#pragma once
#include <vector>
#include <glad\glad.h>
#include <glm\glm.hpp>
using namespace std;
using namespace glm;

class Model
{
private:
	struct Vertex
	{
		vec3 point;
		vec3 normal;
	};

	vector<Vertex> vertices;
	GLuint vertexArray;
	GLuint vertexBuffer;

	void parseObj(const char * objPath);

public:
	Model(const char * objPath);
	~Model();

	void draw() const;
};

