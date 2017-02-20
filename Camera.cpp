#include "Camera.h"

const float Camera::twoPi = two_pi<float>();
const float Camera::rPitchLimit = half_pi<float>() - epsilon<float>();

vec3 Camera::eulerToDir(float yaw, float pitch)
{
	vec3 dir;

	dir.x = cos(yaw) *  cos(pitch);
	dir.y = sin(pitch);
	dir.z = sin(yaw) *  cos(pitch);

	return dir;
}

Camera::Camera(vec3 position, vec3 point, vec3 up)
{
	this->position = position;
	this->direction = normalize(point - position);
	this->up = up;

	this->rPitch = asin(direction.y);
	this->rYaw = acos(direction.x / cos(rPitch));

	viewMatrix = lookAt(position, point, up);
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

void Camera::rotate(float deltaYaw, float deltaPitch)
{
	rYaw = fmodf(rYaw + deltaYaw, twoPi);
	rPitch = min(max(rPitch + deltaPitch, -rPitchLimit), rPitchLimit);;

	direction = eulerToDir(rYaw, rPitch);
	viewMatrix = lookAt(position, position + direction, up);
}
