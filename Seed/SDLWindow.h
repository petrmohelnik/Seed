#pragma once
#include "Input.h"
#include "Time.h"

class SDLWindow
{
public:
	SDLWindow(Input& input, Time& time);

	void CreateWindow(int width, int height);
	void InitializeOpenGL();
	void PollInputs();
	void UpdateTime();
	void Swap();

private:
    SDL_Window* window = nullptr;
	SDL_GLContext context;
	Input& input;
	Time& time;
};

