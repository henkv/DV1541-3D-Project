#pragma once
#include "Shader.h"

class GameObject
{
public:
	typedef size_t uid_t;
	const uid_t UID = UID_BASE++;

	virtual void draw(Shader & shader) = 0;
	virtual void update(float delta) = 0;

private:
	static uid_t UID_BASE;
};

