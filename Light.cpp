#include "Light.h"



Light::Light(vec3 position, vec3 color)
{
	this->position = position;
	this->color = color;
}

Light::~Light()
{
}

void Light::setColor(vec3 color)
{
	this->color = color;
}

void Light::setPosition(vec3 position)
{
	this->position = position;
}

vec3 Light::getColor()
{
	return color;
}

vec3 Light::getPosition()
{
	return position;
}
