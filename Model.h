#pragma once
#include <vector>
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
	std::vector<Vertex> vertices;

	void parseObj(const char * objPath);
public:
	Model(const char * objPath);
	~Model();
};

