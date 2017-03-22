#pragma once
#include <glad\glad.h>
#include <glm\gtc\matrix_transform.hpp>
#include "Mesh.h"
#include "Material.h"
#include "GameObject.h"


class Model : public GameObject
{
private:
	Mesh mesh;
	Material material;

	mat4 worldMatrix;
	vec3 position;

public:
	Model(Mesh & mesh, Material & material);
	~Model();

	void setMesh(Mesh & mesh);
	void setMaterial(Material & material);

	void move(vec3 delta);
	void setPosition(vec3 position);
	vec3 getPosition();



	// Inherited via GameObject
	virtual void draw(Shader & shader) override;

	virtual void drawTesselated(Shader & shader) override;

	virtual void update(float delta) override;

};

