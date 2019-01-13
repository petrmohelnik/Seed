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

void RenderingPipeline::AddLight(std::weak_ptr<Light> light)
{
    lights.push_back(light);
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
    if(auto cameraSharedPtr = mainCamera.lock())
        return cameraSharedPtr.get();
    return nullptr;
}

void RenderingPipeline::CleanComponents()
{
    std::experimental::erase_if(renderers, [](const auto& renderer)
    {
        auto test = renderer.expired();
        return renderer.expired();
    });
    std::experimental::erase_if(renderers, [](const auto& light)
    {
        return light.expired();
    });
    std::experimental::erase_if(renderers, [](const auto& camera)
    {
        return camera.expired();
    });
}
