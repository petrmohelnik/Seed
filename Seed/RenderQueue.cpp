#include "RenderQueue.h"
#include "Renderer.h"
#include "Camera.h"
#include "Engine.h"

RenderQueue::RenderQueue(Camera* camera) : camera(camera)
{
    collisionFunction = [camera](const AABB& aabb) { return aabb.TestFrustum(camera->GetFrustum()); };
}

void RenderQueue::SetCollisionFunction(std::function<bool(const AABB&)> collisionFunction_)
{
    collisionFunction = collisionFunction_;
}

void RenderQueue::Add(Renderer* renderer, int index)
{
    if (collisionFunction(renderer->GetAABB(index)))
        queue.push_back({ renderer, index });
}
