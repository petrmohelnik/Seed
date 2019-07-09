#pragma once

class Renderer;
class Camera;
class Light;
class RenderQueue;
class Transform;
class Skybox;
class Framebuffer;
class Texture;
class SimpleMesh;

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

    enum class TextureSlot : int
    {
        Albedo = 0,
        Normal = 1,
        Height = 2,
        Metallic = 3,
        Emission = 4,
        Occlusion = 5,
        Irradiance = 10,
        Environmental = 11,
        BRDFIntegration = 12
    };

    static void ActivateTexture(TextureSlot textureSlot);

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

    std::unique_ptr<Framebuffer> framebuffer;
    std::unique_ptr<Texture> framebufferTexture;
    std::unique_ptr<SimpleMesh> quad;

    static Camera* mainCamera;
    static Skybox* skybox;
    static GLuint cameraUniform;
    static GLuint lightUniform;
    static GLuint modelUniform;
    static GLuint materialUniform;
};
