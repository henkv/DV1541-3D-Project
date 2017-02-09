#pragma once
#include <glm\glm.hpp>
#include "GameObject.h"


using namespace glm;

class Light : public GameObject
{
private:
	vec3 position;
	vec3 color;

public:
	Light(vec3 position, vec3 color);
	~Light();

	void setColor(vec3 color);
	void setPosition(vec3 position);

	vec3 getColor();
	vec3 getPosition();
};

