#include "Engine.h"

Input Engine::input;
FileSystem Engine::fileSystem;
SDLWindow Engine::window(input);
Time Engine::time(window);
Objects Engine::objects(components, fileSystem);
Components Engine::components(input, time, objects);

Engine::Engine() 
    : sceneDefinition(objects, fileSystem)
    , scene(sceneDefinition, objects, components)
{
}

Time& Engine::GetTime()
{
    return time;
}

Input& Engine::GetInput()
{
    return input;
}

Objects& Engine::GetObjects()
{
    return objects;
}

Components& Engine::GetComponents()
{
    return components;
}

FileSystem& Engine::GetFileSystem()
{
    return fileSystem;
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
