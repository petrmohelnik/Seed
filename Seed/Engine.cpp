#include "Engine.h"

Input Engine::input;
FileSystem Engine::fileSystem;
SDLWindow Engine::window;
Time Engine::time(window);
Objects Engine::objects;
Components Engine::components;
ShaderFactory Engine::shaderFactory;

Engine::Engine() 
    : sceneDefinition()
    , scene(sceneDefinition)
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

ShaderFactory& Engine::GetShaderFactory()
{
    return shaderFactory;
}

SDLWindow& Engine::GetWindow()
{
    return window;
}

void Engine::CreateWindow()
{
	window.CreateWindow(1600, 900);
	window.InitializeOpenGL();
}

void Engine::Work()
{
    components.Initialize();
    scene.Initialize("Default");
    time.InitializeTime();

	while (input.IsGameRunning() && !input.Key(SDLK_ESCAPE))
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

		input.CreateSceneGraph();
		scene.OnFrameUpdate();
        objects.UpdateForDestruction();
		scene.Render();
		window.Swap();
	}
}
