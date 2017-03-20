#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>
using namespace glm;

class Material
{
private:
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	GLfloat specularExponent;
	GLuint diffuseMap;

public:
	Material(vec3 ambientColor = vec3(0, 0, 0)
		, vec3 diffuseColor = vec3(0, 0, 0)
		, vec3 specularColor = vec3(0, 0, 0)
		, GLfloat specularExponent = 0.0f
		, GLuint diffuseMap = 0);
	~Material();

	void setAmbientColor(vec3 ambientColor);
	void setDiffuseColor(vec3 diffuseColor);
	void setSpecularColor(vec3 specularColor);
	void setSpecularExponent(GLfloat specularExponent);
	void setDiffuseMap(GLuint diffuseMap);

	GLuint getDiffuseMap();
};

