#pragma once

class Renderer;

class RenderQueue
{
public:
    RenderQueue();

    void Add(Renderer* renderer, int index);

protected:
    friend class RenderingPipeline;

private:
    std::vector<std::pair<Renderer*, int>> queue;
};

