#include "Engine.h"

Engine::Engine() 
    : window(input, time)
    , components(input, time, objects)
    , scene(objects, components)
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
		scene.OnFrameUpdate();
		scene.CleanObjects();
		scene.Render();
		window.Swap();
	}
}
