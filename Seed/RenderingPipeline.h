#pragma once

class Renderer;

class RenderingPipeline
{
public:
    void AddRenderer(std::weak_ptr<Renderer> renderer);
    void Render();
    void CleanRenderers();

private:
    std::vector<std::weak_ptr<Renderer>> renderers;
};
