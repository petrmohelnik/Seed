#pragma once
#include "SDLWindow.h"
#include "Input.h"
#include "Time.h"

class Engine
{
public:
	Engine();

	void CreateWindow();
	void Work();

private:
	void OnFrameUpdate();
	void Render();

	SDLWindow window;
	Input input;
	Time time;
};

