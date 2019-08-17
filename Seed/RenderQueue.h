#pragma once

class Renderer;
class Camera;

class RenderQueue
{
public:
    RenderQueue(const Camera& camera);

    void Add(Renderer* renderer, int index);

protected:
    friend class RenderingPipeline;

    struct SubmeshRenderer
    {
        Renderer* meshRenderer;
        int submeshIndex;
    };

private:
    const Camera& camera;
    std::vector<SubmeshRenderer> queue;
};

