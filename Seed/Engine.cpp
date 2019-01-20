#include "Engine.h"

Engine::Engine() 
    : window(input, time)
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

	while (input.GameIsRunning())
	{
		window.PollInputs();

		if (input.Pause())
		{
			isPaused = true;
		}
		if (input.Resume())
		{
			isPaused = false;
		}
		if (!isPaused)
		{
			while (time.CurrentTime() - time.DeltaTime() >= time.FixedDeltaTime())
			{
				scene.OnFixedUpdate();
			}
		}

		window.UpdateTime();
		scene.OnFrameUpdate();
        objects.UpdateForDestruction();
		scene.Render();
		window.Swap();
	}
}
