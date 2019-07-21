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
    static void ActivateTexture(TextureSlot textureSlot, Texture* textureToBind);

    void AddRenderer(Renderer* renderer);
    void AddCamera(Camera* camera);
    void AddLight(Light* light);
    static void SetMainCamera(Camera* camera);
    static Camera* MainCamera();
    void CleanComponents();

	void SetSkybox(Skybox* skybox);

private:
    void IntializeGBuffer(int width, int height);
    void RenderCamera(Camera* camera);
    void RenderGBuffer(RenderQueue* queue);
    void RenderLights();
    void RenderToneMapping();
    void RenderSkybox(Camera* camera);
    void RenderToDefaultFrameBuffer(Texture* texture);
    void FillRenderQueue(RenderQueue* queue, Camera* camera);

    std::vector<Renderer*> renderers;
    std::vector<Camera*> cameras;
    std::vector<Light*> lights;
    Transform* rootTransform;

    struct GBuffer
    {
        std::unique_ptr<Framebuffer> buffer;
        std::unique_ptr<Texture> colorTexture;
        std::unique_ptr<Texture> normalTexture;
        std::unique_ptr<Texture> metallicTexture;
        std::unique_ptr<Texture> depthTexture;
    };
    GBuffer gbuffer;
    
    std::unique_ptr<Framebuffer> illuminationBuffer;
    std::unique_ptr<Texture> illuminationTexture;

    std::unique_ptr<Framebuffer> tonemappingBuffer;
    std::unique_ptr<Texture> tonemappingTexture;

    std::unique_ptr<SimpleMesh> quad;

    static Camera* mainCamera;
    static Skybox* skybox;
    static GLuint cameraUniform;
    static GLuint lightUniform;
    static GLuint modelUniform;
    static GLuint materialUniform;
};
