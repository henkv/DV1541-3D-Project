#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <glm\glm.hpp>
#include "Vertex.h"
#include "Mesh.h"
#include "Material.h"
#include "Model.h"

class ObjFile
{
private:
	std::string folder;
	std::string mtllib;

	std::vector<glm::vec3> points;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> vertices;

	void parseObjLine(std::string line);

	void parsePoint(std::stringstream & stream);
	void parseUV(std::stringstream & stream);
	void parseNormal(std::stringstream & stream);
	void parseVertex(std::stringstream & stream);

public:
	ObjFile(std::string filePath);
	~ObjFile();

	Mesh getMesh();
	std::string getMtlPath();
};
