#include "Engine.h"

Engine::Engine() 
    : window(input)
    , time(window)
    , components(input, time, objects)
    , objects(components)
    , sceneDefinition(objects)
    , scene(sceneDefinition, objects, components)
{
}

void Engine::CreateWindow()
{
	window.CreateWindow(1024, 768);
	window.InitializeOpenGL();
}

void Engine::Work()
{
    scene.Initialize("Default");
    time.InitializeTime();

	while (input.GameIsRunning())
	{
		window.PollInputs();
		time.UpdateTime();

		if (input.Pause())
		{
            time.PauseTime();
		}
		if (input.Resume())
		{
            time.UnpauseTime();
		}
		while (time.StartFixedUpdate())
		{
			scene.OnFixedUpdate();
            time.EndFixedUpdate();
		}

		scene.OnFrameUpdate();
        objects.UpdateForDestruction();
		scene.Render();
		window.Swap();
	}
}
