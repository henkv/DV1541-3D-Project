#pragma once
#include <vector>
#include "GameObject.h"
#include "Camera.h"
#include "Model.h"

class GameObjectManager : GameObject
{
private:
	std::vector<GameObject*> objects;

	bool sort(Camera & camera, GameObject* objectLeft, GameObject* objectRight);
public:
	GameObjectManager();
	~GameObjectManager();

	void add(GameObject* object);
	void frontToBackSort(Camera & camera);


	// Inherited via GameObject
	virtual void draw(Shader & shader) override;
	virtual void update(float delta) override;

	virtual void drawTesselated(Shader & shader) override;
};

