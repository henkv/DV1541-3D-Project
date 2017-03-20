#pragma once
#include <glm\glm.hpp>
#include <string>
#include <sstream>
#include "Material.h"

class MtlFile
{
private:
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float specularExponent;
	std::string	diffuseMapPath;

	glm::vec3 parseVec3(std::stringstream & stream);
	float parseFloat(std::stringstream & stream);

	GLuint loadTexture(std::string filePath);

public:
	MtlFile(std::string filePath);
	~MtlFile();

	Material getMaterial();
};

