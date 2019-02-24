#pragma once
#include "SDLWindow.h"
#include "Input.h"
#include "Time.h"
#include "Objects.h"
#include "Components.h"
#include "Scene.h"
#include "SceneDefinition.h"
#include "FileSystem.h"
#include "ShaderFactory.h"

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
    static ShaderFactory& GetShaderFactory();

private:
	static SDLWindow window;
	Scene scene;
    SceneDefinition sceneDefinition;

	static Input input;
	static Time time;
	static Objects objects;
	static Components components;
    static FileSystem fileSystem;
    static ShaderFactory shaderFactory;
};

