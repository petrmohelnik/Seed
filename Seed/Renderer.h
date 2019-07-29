#pragma once
#include "Component.h"
#include "RenderQueue.h"
#include "ShaderFactory.h"

class Input;
class Time;

class Renderer : public Component
{
public:
    Renderer(Object* object);
	virtual ~Renderer() = default;

    virtual void AddToRenderQueueDeferred(RenderQueue& queue) = 0;
    virtual void AddToRenderQueueForward(RenderQueue& queue) = 0;
	virtual void Render(int index) = 0;
    virtual void Render(int index, ShaderFactory::Type shaderType) = 0;

private:
    Input& input;
    Time& time;
};