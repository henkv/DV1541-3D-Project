#include "ObjFile.h"


ObjFile::ObjFile(std::string filePath)
{
	folder = filePath.substr(0, filePath.find_last_of('/'));
	std::ifstream objContent(filePath);
	std::string line;

	if (!objContent)
		throw "file does not exist";

	while (std::getline(objContent, line))
		parseObjLine(line);

	objContent.close();
}

ObjFile::~ObjFile()
{
}



Mesh ObjFile::getMesh()
{
	std::vector<Vertex> vertexVector;

	for (ivec3 & v : vertices)
	{
		vertexVector.push_back({
			points[v.x - 1],
			uvs[v.y - 1],
			normals[v.z - 1]
		});
	}

	return Mesh(vertexVector);
}

std::string ObjFile::getMtlPath()
{
	return folder.size() > 0 ? folder + '/' + mtllib : mtllib;
}

void ObjFile::parseObjLine(std::string line)
{
	std::stringstream lineStream(line);
	std::string linePart;

	if (std::getline(lineStream, linePart, ' '))
	{
		if (linePart == "v") parsePoint(lineStream);
		else if (linePart == "vt") parseUV(lineStream);
		else if (linePart == "vn") parseNormal(lineStream);
		else if (linePart == "f") parseVertex(lineStream);
		else if (linePart == "mtllib")
		{
			std::getline(lineStream, linePart);
			mtllib = linePart;
		}
	}
}

void ObjFile::parsePoint(std::stringstream & stream)
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

void ObjFile::parseUV(std::stringstream & stream)
{
	std::string part;
	glm::vec2 uv;

	getline(stream, part, ' ');
	uv.x = std::stof(part);
	getline(stream, part, ' ');
	uv.y = std::stof(part);

	uvs.push_back(uv);
}

void ObjFile::parseNormal(std::stringstream & stream)
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

void ObjFile::parseVertex(std::stringstream & stream)
{
	std::string part;

	while (getline(stream, part, ' '))
	{
		std::stringstream partStream(part);
		std::string subPart;
		glm::ivec3 vertex;

		getline(partStream, subPart, '/');
		vertex.x = std::stoi(subPart);
		getline(partStream, subPart, '/');
		vertex.y = std::stoi(subPart);
		getline(partStream, subPart, ' ');
		vertex.z = std::stoi(subPart);

		vertices.push_back(vertex);
	}	
}
