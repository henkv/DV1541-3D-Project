#pragma once
#include "Window.h"
#include "Scene.h"

class Engine
{
private:
	Window* window;
	Scene scene;

public:
	Engine();
	~Engine();

	void start();

	void update(float delta);
	void render();
};

