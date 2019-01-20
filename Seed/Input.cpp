#include "Input.h"

bool Input::GameIsRunning()
{
	return true;
}

bool Input::Pause()
{
	return false;
}

bool Input::Resume()
{
	return false;
}

void Input::AddInput(SDL_Event event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		keyPressed.insert(event.key.keysym.sym);
		break;
	}
}

void Input::ClearInputs()
{
	keyPressed.clear();
}

bool Input::KeyPressed(SDL_Keycode key)
{
	return keyPressed.find(key) != std::end(keyPressed);
}
