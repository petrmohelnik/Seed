#include "RenderQueue.h"
#include "Renderer.h"
#include "Camera.h"
#include "Engine.h"

RenderQueue::RenderQueue(const Camera& camera) : camera(camera)
{
}

void RenderQueue::Add(Renderer* renderer, int index)
{
    if (camera.IsInsideFrustum(renderer->GetAABB(index)))
        queue.push_back({ renderer, index });
}
