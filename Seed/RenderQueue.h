#pragma once

class Renderer;
class Camera;

class RenderQueue
{
public:
    RenderQueue() = default;
    RenderQueue(Camera* camera);

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
    std::vector<SubmeshRenderer> queue;
};

