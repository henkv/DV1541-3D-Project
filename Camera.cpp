#include "Camera.h"



Camera::Camera(vec3 position, vec3 direction, vec3 up)
{
	this->position = position;
	this->direction = direction;
	this->up = up;

	viewMatrix = lookAt(position, position + direction, up);
	projectionMatrix = perspective(pi<float>() * 0.2f, 8.f / 6.f, 0.1f, 100.f);
}

Camera::~Camera()
{
}

vec3 Camera::getUp()
{
	return up;
}

vec3 Camera::getPosition()
{
	return position;
}

vec3 Camera::getDirection()
{
	return direction;
}

mat4 Camera::getViewMatrix()
{
	return viewMatrix;
}

mat4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::setUp(vec3 up)
{
	this->up = up;
}

void Camera::setPosition(vec3 position)
{
	this->position = position;
}

void Camera::setDirection(vec3 direction)
{
	this->direction = direction;
}

void Camera::setProjectionMatrix(mat4 projectionMatrix)
{
	this->projectionMatrix = projectionMatrix;
}

void Camera::move(vec3 delta)
{
	position += delta;
	viewMatrix = lookAt(position, position + direction, up);
}
