#pragma once

class Input
{
public:
	Input();
	void AddInput(SDL_Event event);
	void ClearInputs();
	static bool IsKeyPressed(SDL_Keycode key);

private:
	static std::set<SDL_Keycode> keyPressed;
};

