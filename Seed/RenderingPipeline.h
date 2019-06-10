#pragma once

class Renderer;
class Camera;
class Light;
class RenderQueue;
class Transform;
class Skybox;

class RenderingPipeline
{
public:
    void SetRootTransform(Transform* root);

    void Initialize();
    void Render();

    static void BindCameraUniform();
    static void BindLightUniform();
    static void BindModelUniform();
    static void BindMaterialUniform();
    static void BindSkyboxEnvironmentalMap();

    void AddRenderer(Renderer* renderer);
    void AddCamera(Camera* camera);
    void AddLight(Light* light);
    static void SetMainCamera(Camera* camera);
    static Camera* MainCamera();
    void CleanComponents();

	void SetSkybox(Skybox* skybox);

private:
    void RenderCamera(Camera* camera);
    void FillRenderQueue(RenderQueue* queue, Camera* camera);

    std::vector<Renderer*> renderers;
    std::vector<Camera*> cameras;
    std::vector<Light*> lights;
    Transform* rootTransform;

    static Camera* mainCamera;
    static Skybox* skybox;
    static GLuint cameraUniform;
    static GLuint lightUniform;
    static GLuint modelUniform;
    static GLuint materialUniform;
};
