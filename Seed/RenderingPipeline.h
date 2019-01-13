#pragma once

class Renderer;
class Camera;
class Light;

class RenderingPipeline
{
public:
    void AddRenderer(std::weak_ptr<Renderer> renderer);
    void AddCamera(std::weak_ptr<Camera> camera);
    void AddLight(std::weak_ptr<Light> light);
    void Render();
    static void SetMainCamera(std::weak_ptr<Camera> camera);
    static Camera* MainCamera();
    void CleanComponents();

private:
    std::vector<std::weak_ptr<Renderer>> renderers;
    std::vector<std::weak_ptr<Camera>> cameras;
    static std::weak_ptr<Camera> mainCamera;
    std::vector<std::weak_ptr<Light>> lights;
};
