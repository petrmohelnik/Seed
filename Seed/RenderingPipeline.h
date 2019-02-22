#pragma once

class Renderer;
class Camera;
class Light;

class RenderingPipeline
{
public:
    void Initialize();

    void AddRenderer(Renderer* renderer);
    void AddCamera(Camera* camera);
    void AddLight(Light* light);
    void Render();
    static void SetMainCamera(Camera* camera);
    static Camera* MainCamera();
    void CleanComponents();

private:
    std::vector<Renderer*> renderers;
    std::vector<Camera*> cameras;
    static Camera* mainCamera;
    std::vector<Light*> lights;
};
