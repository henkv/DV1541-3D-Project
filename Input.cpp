#include "Input.h"

glm::vec2 Input::mousePos = glm::vec2();
bool Input::keys[1024] = { false };
bool Input::mouseButtons[8] = { false };

bool Input::getKey(Key keyCode)
{
	return keys[keyCode];
}

void Input::setKey(Key keyCode, bool state)
{
	keys[keyCode] = state;
}

bool Input::getMouseButton(Mouse keyCode)
{
	return mouseButtons[keyCode];
}

void Input::setMouseButton(Mouse keyCode, bool state)
{
	mouseButtons[keyCode] = state;
}

void Input::setMousePos(float x, float y)
{
	mousePos.x = x;
	mousePos.y = y;
}

glm::vec2 Input::getMousePos()
{
	return mousePos;
}
