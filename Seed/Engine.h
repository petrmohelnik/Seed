#pragma once
#include "SDLWindow.h"
#include "Input.h"
#include "Time.h"
#include "Objects.h"
#include "Components.h"
#include "Scene.h"

class Engine
{
public:
	Engine();

	void CreateWindow();
	void Work();

private:
	SDLWindow window;
	Scene scene;

	Input input;
	Time time;
	Objects objects;
	Components components;
};

