#pragma once
#include <glm\glm.hpp>
using namespace glm;

class Material
{
private:
	struct Block
	{
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		float transparency;
	};

public:
	Material();
	~Material();
};

