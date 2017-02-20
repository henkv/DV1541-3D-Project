#include "Input.h"

bool Input::keys[1024] = { false };

bool Input::getKey(Key keyCode)
{
	return keys[keyCode];
}

void Input::setKey(Key keyCode, bool state)
{
	keys[keyCode] = state;
}
