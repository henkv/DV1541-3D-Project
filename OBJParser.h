#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <glm\glm.hpp>


class OBJParser
{
private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> vertices;

	void parseObjFile(const char * filePath);
	void parseObjLine(std::string line);

	void parsePoint(std::stringstream & stream);
	void parseUV(std::stringstream & stream);
	void parseNormal(std::stringstream & stream);
	void parseFace(std::stringstream & stream);

public:
	OBJParser();
	~OBJParser();
};

