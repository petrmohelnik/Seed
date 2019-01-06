#pragma once

class Input
{
public:
	bool IsKeyPressed(SDL_Keycode key);

protected:
    friend class SDLWindow;
	void AddInput(SDL_Event event);
	void ClearInputs();

private:
	std::set<SDL_Keycode> keyPressed;
};

