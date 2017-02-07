#pragma once
#include "Shader.h"

class GameObject
{
public:
	typedef size_t uid_t;

private: 
	static uid_t UID_BASE;

public:
	const uid_t UID = UID_BASE++;
	virtual void draw(Shader & shader) = 0;
	virtual void update(float delta) = 0;
};

GameObject::uid_t GameObject::UID_BASE = 0;