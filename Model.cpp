#include "Model.h"
#include <fstream>
#include <sstream>
#include <string>

#define OFFSET(x) ((GLvoid*)x)

void Model::parseObj(const char * objPath)
{
	std::vector<vec3> points;
	std::vector<vec3> normals;

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
			vec3 point;

			getline(lineStream, part, ' ');
			point.x = stof(part);
			getline(lineStream, part, ' ');
			point.y = stof(part);
			getline(lineStream, part, ' ');
			point.z = stof(part);

			points.push_back(point);
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
				vertex.point = points.at(stoi(part) - 1);

				getline(partStream, part, '/');
				getline(partStream, part, '/');
				vertex.normal = normals.at(stoi(part) - 1);

				vertices.push_back(vertex);
			}
		}
	}

	file.close();
	vertices.shrink_to_fit();
}

Model::Model(const char * objPath)
{
	parseObj(objPath);

	glGenVertexArrays(1, &vertexArray);
	glGenBuffers(1, &vertexBuffer);

	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // point
	glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(Vertex), OFFSET(0));

	glEnableVertexAttribArray(1); // normal
	glVertexAttribPointer(0, 3, GL_FLOAT, TRUE, sizeof(Vertex), OFFSET(sizeof(vec3)));

	glBindVertexArray(0);
}

Model::~Model()
{
}

void Model::draw() const
{
	glBindVertexArray(vertexArray);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
}
