#include "Engine.h"

Engine::Engine() : window(input, time)
{
}

void Engine::CreateWindow()
{
	window.CreateWindow(1024, 768);
	window.InitializeOpenGL();
}

void Engine::Work()
{
	while (1)
	{
		window.PollInputs();
		window.UpdateTime();
		OnFrameUpdate();
		Render();
		window.Swap();
	}
}

void Engine::OnFrameUpdate()
{
}

void Engine::Render()
{
}
