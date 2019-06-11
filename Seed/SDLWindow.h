#pragma once
#include "Input.h"

class SDLWindow
{
public:
	SDLWindow();

protected:
    friend class Time;
    friend class Engine;
    friend class TextureCubeMap;
    friend class EnvironmentalMap;

	void CreateWindow(int width, int height);
	void InitializeOpenGL();
	void PollInputs();
	Uint32 GetTime();
	void Swap();
    void ResetViewport();

private:
    SDL_Window* window = nullptr;
	SDL_GLContext context;
	Input& input;
};

