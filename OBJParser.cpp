#include "OBJParser.h"


void OBJParser::parseObjFile(const char * filePath)
{
	points.clear();
	uvs.clear();
	normals.clear();
	vertices.clear();

	std::ifstream objContent(filePath);
	std::string line;

	if (std::getline(objContent, line))
		parseObjLine(line);

	objContent.close();
}

void OBJParser::parseObjLine(std::string line)
{
	std::stringstream lineStream(line);
	std::string linePart;

	if (std::getline(lineStream, linePart, ' '))
	{
		if (linePart == "v") parsePoint(lineStream);
		else if (linePart == "vt") parseUV(lineStream);
		else if (linePart == "vn") parseNormal(lineStream);
		else if (linePart == "f") parseFace(lineStream);
	}
}

void OBJParser::parsePoint(std::stringstream & stream)
{
	std::string part;
	glm::vec3 point;

	getline(stream, part, ' ');
	point.x = std::stof(part);
	getline(stream, part, ' ');
	point.y = std::stof(part);
	getline(stream, part, ' ');
	point.z = std::stof(part);

	points.push_back(point);
}

void OBJParser::parseUV(std::stringstream & stream)
{
	std::string part;
	glm::vec2 uv;

	getline(stream, part, ' ');
	uv.x = std::stof(part);
	getline(stream, part, ' ');
	uv.y = std::stof(part);

	uvs.push_back(uv);
}

void OBJParser::parseNormal(std::stringstream & stream)
{
	std::string part;
	glm::vec3 normal;

	getline(stream, part, ' ');
	normal.x = std::stof(part);
	getline(stream, part, ' ');
	normal.y = std::stof(part);
	getline(stream, part, ' ');
	normal.z = std::stof(part);

	normals.push_back(normal);
}

void OBJParser::parseFace(std::stringstream & stream)
{
	std::string part;

	while (getline(stream, part))
	{
		std::stringstream partStream(part);
		std::string subPart;
		glm::ivec3 vertex;

		getline(partStream, subPart, '/');
		vertex.x = std::stof(part);
		getline(partStream, subPart, '/');
		vertex.y = std::stof(part);
		getline(partStream, subPart, ' ');
		vertex.z = std::stof(part);

		vertices.push_back(vertex);
	}	
}

OBJParser::OBJParser()
{
}


OBJParser::~OBJParser()
{
}
