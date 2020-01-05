#pragma once
#include "Input.h"

class SDLWindow
{
public:
	SDLWindow();
	~SDLWindow();

    glm::uvec2 GetWindowSize();

protected:
    friend class Time;
    friend class Engine;
    friend class Framebuffer;
    friend class RenderingPipeline;

	void CreateWindow(int width, int height);
	void InitializeOpenGL();
	void PollInputs();
	double GetTime();
	void Swap();
    void ResetViewport();

private:
    SDL_Window* window = nullptr;
	SDL_GLContext context;
    ImGuiIO* imguiIO;
	Input& input;
};

