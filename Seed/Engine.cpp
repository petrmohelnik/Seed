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

void Engine::CreateWindow()
{
	window.CreateWindow(1280, 720);
	window.InitializeOpenGL();
}

void Engine::Work()
{
    scene.Initialize("Default");
    time.InitializeTime();
    components.Initialize();

	while (input.GameIsRunning() && !input.Key(SDLK_ESCAPE))
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
