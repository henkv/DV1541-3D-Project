#pragma once
#include "GameObject.h"
class Scene : GameObject
{
public:
	Scene();
	~Scene();

	// Inherited via GameObject
	virtual void render() override;
	virtual void update(float delta) override;
};

