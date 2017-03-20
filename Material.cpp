#include "Material.h"




Material::Material(vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, GLfloat specularExponent, GLuint diffuseMap)
	: ambientColor(ambientColor)
	, diffuseColor(diffuseColor)
	, specularColor(specularColor)
	, specularExponent(specularExponent)
	, diffuseMap(diffuseMap)
{}

Material::~Material()
{
}

void Material::setAmbientColor(vec3 ambientColor)
{
	this->ambientColor = ambientColor;
}

void Material::setDiffuseColor(vec3 diffuseColor)
{
	this->diffuseColor = diffuseColor;
}

void Material::setSpecularColor(vec3 specularColor)
{
	this->specularColor = specularColor;
}

void Material::setSpecularExponent(GLfloat specularExponent)
{
	this->specularExponent = specularExponent;
}


void Material::setDiffuseMap(GLuint diffuseMap)
{
	this->diffuseMap = diffuseMap;
}

GLuint Material::getDiffuseMap()
{
	return diffuseMap;
}
