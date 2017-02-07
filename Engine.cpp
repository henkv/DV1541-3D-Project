#include "Engine.h"



Engine::Engine()
{

}


Engine::~Engine()
{
}

void Engine::start()
{
	double lastFrame = window->getTime();
	while (window->isOpen())
	{
		update(window->getTime() - lastFrame);
		lastFrame = window->getTime();

		window->clear();;
		render();		
		window->swapBuffers();
	}
}


void Engine::update(float delta)
{
	scene.update(delta);
}

void Engine::render()
{
	scene.render();
}
