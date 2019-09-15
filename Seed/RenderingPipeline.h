#pragma once
#include "Texture.h"

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
        Shadow = 6,
        ShadowLerp = 7,
        Irradiance = 10,
        Environmental = 11,
        BRDFIntegration = 12
    };

    static void ActivateTexture(TextureSlot textureSlot);
    static void ActivateTexture(TextureSlot textureSlot, Texture* textureToBind);
    static void BindSampler(TextureSlot textureSlot, GLuint sampler);

    void AddRenderer(Renderer* renderer);
    void AddCamera(Camera* camera);
    void AddLight(Light* light);
    static void SetMainCamera(Camera* camera);
    static Camera* MainCamera();
    void CleanComponents();

	void SetSkybox(Skybox* skybox);

private:
    void IntializeTextures(int width, int height);
    void IntializeBuffers(int width, int height);
    void SetShadowSamplerParameters(GLuint sampler, GLuint compareMode = GL_NONE);
    void RenderCamera(Camera& camera);
    void RenderGBuffer(RenderQueue& queue);
    void RenderGlobalIllumination();
    void RenderShadowMap(const Light& light);
    void RenderLights(Camera& camera);
    void BlendIllumination();
    void RenderToneMapping();
    void RenderForward(RenderQueue& queue);
    void RenderSkybox(Camera& camera);
    void RenderToDefaultFrameBuffer(Texture& texture);
    void FillRenderQueues(RenderQueue& deferredQueue, RenderQueue& forwardQueue, const Camera& camera);

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
        std::unique_ptr<Texture> depthStencilTexture;
    };
    GBuffer gbuffer;
    
    struct ShadowMap
    {
        ~ShadowMap();
        std::unique_ptr<Framebuffer> buffer;
        std::unique_ptr<Texture> texture2D;
        GLuint sampler2DShadow[2];
    };
    ShadowMap shadowMap;

    std::unique_ptr<Framebuffer> lightsIlluminationBuffer;
    std::unique_ptr<Texture> lightsIlluminationTexture;

    std::unique_ptr<Framebuffer> globalIlluminationBuffer;
    std::unique_ptr<Texture> globalIlluminationTexture;

    std::unique_ptr<Framebuffer> tonemappingBuffer;
    std::unique_ptr<Texture> tonemappingTexture;

    std::unique_ptr<SimpleMesh> quad;
    std::unique_ptr<SimpleMesh> boundingSphere;

    static Camera* mainCamera;
    static Skybox* skybox;
    static GLuint cameraUniform;
    static GLuint lightUniform;
    static GLuint modelUniform;
    static GLuint materialUniform;
};
