#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
using namespace glm;

class Camera
{
private:
	static const float twoPi;
	static const float rPitchLimit;

	vec3 up;
	vec3 position;
	vec3 direction;

	float rYaw;
	float rPitch;
	
	mat4 viewMatrix;
	mat4 projectionMatrix;

	vec3 eulerToDir(float yaw, float pitch);

public:
	Camera(vec3 position, vec3 direction, vec3 up);
	~Camera();

	vec3 getUp();
	vec3 getPosition();
	vec3 getDirection();

	mat4 getViewMatrix();
	mat4 getProjectionMatrix();

	void setUp(vec3 up);
	void setPosition(vec3 position);
	void setDirection(vec3 direction);
	void setProjectionMatrix(mat4 projectionMatrix);

	void move(vec3 delta);
	void rotate(float deltaYaw, float deltaPitch);

	void update(float delta);
};

