#include "RenderingPipeline.h"
#include "Renderer.h"
#include "Camera.h"
#include "Light.h"

Camera* RenderingPipeline::mainCamera;

void RenderingPipeline::AddRenderer(Renderer* renderer)
{
    renderers.push_back(renderer);
}

void RenderingPipeline::AddCamera(Camera* camera)
{
    cameras.push_back(camera);
}

void RenderingPipeline::AddLight(Light* light)
{
    lights.push_back(light);
}

void RenderingPipeline::Render()
{
    for (const auto& renderer : renderers)
    {
        renderer->Render();
    }
}

void RenderingPipeline::SetMainCamera(Camera* camera)
{
    mainCamera = camera;
}

Camera* RenderingPipeline::MainCamera()
{
    return mainCamera;
}

void RenderingPipeline::CleanComponents()
{
    std::experimental::erase_if(renderers, [](const auto renderer)
    {
        return renderer->IsRegisteredForDestruction();
    });
    std::experimental::erase_if(lights, [](const auto light)
    {
        return light->IsRegisteredForDestruction();
    });
    std::experimental::erase_if(cameras, [](const auto camera)
    {
        return camera->IsRegisteredForDestruction();
    });
    if (mainCamera && mainCamera->IsRegisteredForDestruction())
        mainCamera = nullptr;
}
