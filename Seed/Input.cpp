#include "Input.h"

std::set<SDL_Keycode> Input::keyPressed;

Input::Input()
{
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

bool Input::IsKeyPressed(SDL_Keycode key)
{
	return keyPressed.find(key) != std::end(keyPressed);
}
