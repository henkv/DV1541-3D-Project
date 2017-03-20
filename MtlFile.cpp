#include "MtlFile.h"
#include <fstream>
#include <SOIL.h>


glm::vec3 MtlFile::parseVec3(std::stringstream & stream)
{
	std::string str;
	glm::vec3 value;

	std::getline(stream, str, ' ');
	value.x = stof(str);
	std::getline(stream, str, ' ');
	value.y = stof(str);
	std::getline(stream, str, ' ');
	value.z = stof(str);

	return value;
}

float MtlFile::parseFloat(std::stringstream & stream)
{
	std::string str;
	std::getline(stream, str, ' ');
	return stof(str);
}

GLuint MtlFile::loadTexture(std::string filePath)
{
	GLuint tex_2d = SOIL_load_OGL_texture
	(
		filePath.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_COMPRESS_TO_DXT
	);

	if (0 == tex_2d)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}

	return tex_2d;
}

MtlFile::MtlFile(std::string filePath)
	: specularExponent(0.0f)
	, diffuseMapPath("")
{
	std::string folder = filePath.substr(0, filePath.find_last_of('/'));

	std::ifstream fileContent(filePath);
	if (!fileContent) throw "file does not exist";

	std::string str;
	std::stringstream stream;
	while (std::getline(fileContent, str))
	{
		stream.clear();
		stream.str(str);
		std::getline(stream, str, ' ');

			 if (str == "Ka") ambientColor = parseVec3(stream);
		else if (str == "Kd") diffuseColor = parseVec3(stream);
		else if (str == "Ks") specularColor = parseVec3(stream);
		else if (str == "Ns") specularExponent = parseFloat(stream);
		else if (str == "map_Kd")
		{
			std::getline(stream, str);
			diffuseMapPath = folder.size() > 0 ? folder + "/" + str : str;
		}
	}
}

MtlFile::~MtlFile()
{
}

Material MtlFile::getMaterial()
{
	return Material(ambientColor, diffuseColor, specularColor, specularExponent, loadTexture(diffuseMapPath));
}
