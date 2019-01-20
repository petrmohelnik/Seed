#pragma once
#include "Input.h"

class SDLWindow
{
public:
	SDLWindow(Input& input);

	void CreateWindow(int width, int height);
	void InitializeOpenGL();
	void PollInputs();
	Uint32 GetTime();
	void Swap();

private:
    SDL_Window* window = nullptr;
	SDL_GLContext context;
	Input& input;
};

