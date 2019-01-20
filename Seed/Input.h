#pragma once

class Input
{
public:
	bool GameIsRunning();
	bool Pause();
	bool Resume();
	bool KeyPressed(SDL_Keycode key);

protected:
    friend class SDLWindow;
	void AddInput(SDL_Event event);
	void ClearInputs();

private:
	std::set<SDL_Keycode> keyPressed;
};

