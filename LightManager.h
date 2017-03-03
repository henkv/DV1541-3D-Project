#pragma once
#include <glad\glad.h>
#include <vector>
#include <glm\glm.hpp>
#include "GameObject.h"
//#include "Light.h"
using namespace glm;

class LightManager : public GameObject
{
public:
	struct Light
	{
		vec3 position;
		GLbyte _1[4];
		vec3 color;
		GLbyte _2[4];

		Light() {}
		Light(vec3 position, vec3 color) 
			: position(position)
			, color(color) {}
	};

	struct
	{
		Light lights[100];
		GLint nrOfLights;
	} bufferData;


	GLuint buffer;

public:
	LightManager();
	~LightManager();

	void add(Light & light);

	// Inherited via GameObject
	virtual void draw(Shader & shader) override;
	virtual void update(float delta) override;
};

