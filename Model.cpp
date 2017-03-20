#include "Model.h"
#include <fstream>
#include <sstream>
#include <string>


Model::Model(Mesh & mesh, Material & material)
	: mesh(mesh)
	, material(material)
{}

Model::~Model()
{
}

void Model::setMesh(Mesh & mesh)
{
	this->mesh = mesh;
}

void Model::setMaterial(Material & material)
{
	this->material = material;
}

void Model::move(vec3 delta)
{
	position += delta;
	worldMatrix = translate(worldMatrix, delta);
}

void Model::setPosition(vec3 newPosition)
{
	worldMatrix = translate(worldMatrix, newPosition - position);
	this->position = newPosition;
}

vec3 Model::getPosition()
{
	return position;
}

void Model::draw(Shader & shader)
{
	shader.setUniform("worldMatrix", worldMatrix);
	shader.setTexture2D(0, "diffuseMap", material.getDiffuseMap());
	mesh.draw();
}

void Model::update(float delta)
{
}
