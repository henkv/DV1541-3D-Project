#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <glad\glad.h>

const size_t Mesh::VertexSize = sizeof(Vertex);
const void * Mesh::PositionOffset = (void *)offsetof(Vertex, position);
const void * Mesh::NormalOffset   = (void *)offsetof(Vertex, normal);
const void * Mesh::UVOffset		  = (void *)offsetof(Vertex, uv);

void Mesh::parseObj(const char * objPath)
{
	vertices.clear();
	vector<vec3> positions;
	vector<vec3> normals;

	ifstream file(objPath);

	string line;
	string part;
	istringstream lineStream;
	istringstream partStream;

	while (getline(file, line))
	{
		lineStream.clear();
		lineStream.str(line);
		getline(lineStream, part, ' ');

		if (part == "v")
		{
			vec3 position;

			getline(lineStream, part, ' ');
			position.x = stof(part);

			getline(lineStream, part, ' ');
			position.y = stof(part);

			getline(lineStream, part, ' ');
			position.z = stof(part);

			positions.push_back(position);
		}
		else if (part == "vn")
		{
			vec3 normal;

			getline(lineStream, part, ' ');
			normal.x = stof(part);

			getline(lineStream, part, ' ');
			normal.y = stof(part);

			getline(lineStream, part, ' ');
			normal.z = stof(part);

			normals.push_back(normal);
		}
		else if (part == "f")
		{
			Vertex vertex;

			for (int i = 0; i < 3; i++)
			{
				getline(lineStream, part, ' ');
				partStream.clear();
				partStream.str(part);

				getline(partStream, part, '/');
				vertex.position = positions[stoi(part) - 1];

				getline(partStream, part, '/');
				//vertex texture uv

				getline(partStream, part, '/');
				vertex.normal = normals[stoi(part) - 1];

				vertices.push_back(vertex);
			}
		}
	}

	file.close();
	vertices.shrink_to_fit();
}

const size_t Mesh::size() const
{
	return vertices.size();
}

const size_t Mesh::bufferSize() const
{
	return sizeof(Vertex) * vertices.size();
}

const void * Mesh::bufferData() const
{
	return &vertices[0];
}

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}
