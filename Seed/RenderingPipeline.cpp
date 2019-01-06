#include "RenderingPipeline.h"
#include "Renderer.h"

void RenderingPipeline::AddRenderer(std::weak_ptr<Renderer> renderer)
{
    renderers.push_back(renderer);
}

void RenderingPipeline::Render()
{
    for (const auto& renderer : renderers)
    {
        renderer.lock()->Render();
    }
}

void RenderingPipeline::CleanRenderers()
{
    std::remove_if(std::begin(renderers), std::end(renderers), [](const auto& renderer)
    {
        return renderer.expired();
    });
}
