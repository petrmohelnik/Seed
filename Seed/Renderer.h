#pragma once
#include "Component.h"

class Input;
class Time;

class Renderer : public Component
{
public:
    Renderer(Object* object);
	virtual ~Renderer() = default;

	virtual void Render() = 0;

private:
    Input& input;
    Time& time;
};