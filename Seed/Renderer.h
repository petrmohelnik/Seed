#pragma once
#include "Component.h"
#include "RenderQueue.h"

class Input;
class Time;

class Renderer : public Component
{
public:
    Renderer(Object* object);
	virtual ~Renderer() = default;

    virtual void AddToRenderQueue(RenderQueue* queue) = 0;
	virtual void Render(int index) = 0;

private:
    Input& input;
    Time& time;
};