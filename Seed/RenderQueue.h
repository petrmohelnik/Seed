#pragma once

class Renderer;
class Camera;
class AABB;

class RenderQueue
{
public:
    RenderQueue() = default;
    RenderQueue(Camera* camera);

    void SetCollisionFunction(std::function<bool(const AABB&)> collisionFunction);

    void Add(Renderer* renderer, int index);

protected:
    friend class RenderingPipeline;

    struct SubmeshRenderer
    {
        Renderer* meshRenderer;
        int submeshIndex;
    };

private:
    Camera* camera = nullptr;
    std::function<bool(const AABB&)> collisionFunction = [](const AABB&) { return true; };

    std::vector<SubmeshRenderer> queue;
};

