#pragma once
#include <vector>
#include "GameObject.h"

class GameObjectManager : GameObject
{
private:
	std::vector<GameObject*> objects;
public:
	GameObjectManager();
	~GameObjectManager();

	void add(GameObject* object);

	// Inherited via GameObject
	virtual void draw(Shader & shader) override;
	virtual void update(float delta) override;
};

