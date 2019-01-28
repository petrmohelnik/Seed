#pragma once
#include "SDLWindow.h"
#include "Input.h"
#include "Time.h"
#include "Objects.h"
#include "Components.h"
#include "Scene.h"
#include "SceneDefinition.h"
#include "FileSystem.h"

class Engine
{
public:
	Engine();

	void CreateWindow();
	void Work();

    static Time& GetTime();
    static Input& GetInput();
    static Objects& GetObjects();
    static Components& GetComponents();
    static FileSystem& GetFileSystem();

private:
	static SDLWindow window;
	Scene scene;
    SceneDefinition sceneDefinition;

	static Input input;
	static Time time;
	static Objects objects;
	static Components components;
    static FileSystem fileSystem;
};

