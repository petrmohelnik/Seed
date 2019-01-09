#include "RenderingPipeline.h"
#include "Renderer.h"

std::weak_ptr<Camera> RenderingPipeline::mainCamera;

void RenderingPipeline::AddRenderer(std::weak_ptr<Renderer> renderer)
{
    renderers.push_back(renderer);
}

void RenderingPipeline::AddCamera(std::weak_ptr<Camera> camera)
{
    cameras.push_back(camera);
}

void RenderingPipeline::Render()
{
    for (const auto& renderer : renderers)
    {
        renderer.lock()->Render();
    }
}

void RenderingPipeline::SetMainCamera(std::weak_ptr<Camera> camera)
{
    mainCamera = camera;
}

Camera* RenderingPipeline::MainCamera()
{
    return mainCamera.lock().get();
}

void RenderingPipeline::CleanRenderers()
{
    std::remove_if(std::begin(renderers), std::end(renderers), [](const auto& renderer)
    {
        return renderer.expired();
    });
}
