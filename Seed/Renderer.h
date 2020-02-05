#pragma once
#include "Component.h"
#include "RenderQueue.h"
#include "ShaderFactory.h"
#include "AABB.h"

class Renderer : public Component
{
public:
    using Component::Component;

    virtual AABB GetAABB(int index) = 0;
    virtual void AddToRenderQueueDeferred(RenderQueue& queue) = 0;
    virtual void AddToRenderQueueForward(RenderQueue& queue) = 0;
    virtual void Render(int index) = 0;
    virtual void Render(int index, ShaderFactory::Type shaderType) = 0;
    void SetCastShadow(bool castShadow);
    bool GetCastShadow();

private:
    bool castShadow = true;
};