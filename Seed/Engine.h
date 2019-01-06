#pragma once
#include "SDLWindow.h"
#include "Input.h"
#include "Time.h"
#include "Scene.h"

class Engine
{
public:
	Engine();

	void CreateWindow();
	void Work();

private:
	SDLWindow window;
	Input input;
	Time time;

	Scene* activeScene;
	std::vector<std::unique_ptr<Scene>> scenes;
};

