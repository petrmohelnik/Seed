#include "RenderingPipeline.h"
#include "Engine.h"
#include "RenderQueue.h"
#include "Renderer.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "Skybox.h"
#include "EnvironmentalMap.h"
#include "Framebuffer.h"
#include "Texture.h"
#include "TextureCubeMap.h"
#include "SimpleMesh.h"

Camera* RenderingPipeline::mainCamera = nullptr;
Skybox* RenderingPipeline::skybox = nullptr;
GLuint RenderingPipeline::cameraUniform;
GLuint RenderingPipeline::lightUniform;
GLuint RenderingPipeline::modelUniform;
GLuint RenderingPipeline::materialUniform;

void RenderingPipeline::SetRootTransform(Transform* root)
{
    rootTransform = root;
}

void RenderingPipeline::Initialize()
{
    auto windowSize = Engine::GetWindow().GetWindowSize();
    auto width = windowSize.first;
    auto height = windowSize.second;

    IntializeTextures(width, height);
    IntializeBuffers(width, height);

    quad = std::make_unique<SimpleMesh>(SimpleMesh::Shape::Quad);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glGenBuffers(1, &cameraUniform);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, cameraUniform);

    glGenBuffers(1, &lightUniform);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightUniform);

    glGenBuffers(1, &modelUniform);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, modelUniform);

    glGenBuffers(1, &materialUniform);
    glBindBufferBase(GL_UNIFORM_BUFFER, 3, materialUniform);
}

void RenderingPipeline::IntializeTextures(int width, int height)
{
    gbuffer.colorTexture = std::make_unique<Texture>();
    gbuffer.colorTexture->GenerateTexture(GL_CLAMP_TO_EDGE, GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE);
    gbuffer.normalTexture = std::make_unique<Texture>();
    gbuffer.normalTexture->GenerateTexture(GL_CLAMP_TO_EDGE, GL_RGBA16F, width, height, GL_RGBA, GL_FLOAT);
    gbuffer.metallicTexture = std::make_unique<Texture>();
    gbuffer.metallicTexture->GenerateTexture(GL_CLAMP_TO_EDGE, GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE);
    gbuffer.depthTexture = std::make_unique<Texture>();
    gbuffer.depthTexture->GenerateTexture(GL_CLAMP_TO_EDGE, GL_DEPTH_COMPONENT32, width, height, GL_DEPTH_COMPONENT, GL_FLOAT);

    lightsIlluminationTexture = std::make_unique<Texture>();
    lightsIlluminationTexture->GenerateTexture(GL_CLAMP_TO_EDGE, GL_RGB16F, width, height, GL_RGB, GL_FLOAT);

    globalIlluminationTexture = std::make_unique<Texture>();
    globalIlluminationTexture->GenerateTexture(GL_CLAMP_TO_EDGE, GL_RGB16F, width, height, GL_RGB, GL_FLOAT);

    tonemappingTexture = std::make_unique<Texture>();
    tonemappingTexture->GenerateTexture(GL_CLAMP_TO_EDGE, GL_RGB8, width, height, GL_RGB, GL_UNSIGNED_BYTE);
}

void RenderingPipeline::IntializeBuffers(int width, int height)
{
    gbuffer.buffer = std::make_unique<Framebuffer>(width, height);
    gbuffer.buffer->AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gbuffer.colorTexture->texture);
    gbuffer.buffer->AttachTexture(GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gbuffer.normalTexture->texture);
    gbuffer.buffer->AttachTexture(GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gbuffer.metallicTexture->texture);
    gbuffer.buffer->AttachTexture(GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, lightsIlluminationTexture->texture);
    gbuffer.buffer->AttachTexture(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gbuffer.depthTexture->texture);
    gbuffer.buffer->SetDrawBuffers({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 });

    lightsIlluminationBuffer = std::make_unique<Framebuffer>(width, height);
    lightsIlluminationBuffer->AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightsIlluminationTexture->texture);
    lightsIlluminationBuffer->AttachTexture(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gbuffer.depthTexture->texture);

    globalIlluminationBuffer = std::make_unique<Framebuffer>(width, height);
    globalIlluminationBuffer->AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, globalIlluminationTexture->texture);

    tonemappingBuffer = std::make_unique<Framebuffer>(width, height);
    tonemappingBuffer->AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tonemappingTexture->texture);
    tonemappingBuffer->AttachTexture(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gbuffer.depthTexture->texture);
}

void RenderingPipeline::BindCameraUniform()
{
    glBindBuffer(GL_UNIFORM_BUFFER, cameraUniform);
}

void RenderingPipeline::BindLightUniform()
{
    glBindBuffer(GL_UNIFORM_BUFFER, lightUniform);
}

void RenderingPipeline::BindModelUniform()
{
    glBindBuffer(GL_UNIFORM_BUFFER, modelUniform);
}

void RenderingPipeline::BindMaterialUniform()
{
    glBindBuffer(GL_UNIFORM_BUFFER, materialUniform);
}

void RenderingPipeline::BindSkyboxEnvironmentalMap()
{
    if (skybox)
    {
        auto environmentalMap = skybox->GetEnvironmentalMapRawPtr();

        ActivateTexture(TextureSlot::Irradiance);
        environmentalMap->BindIrradiance();
        ActivateTexture(TextureSlot::Environmental);
        environmentalMap->BindEnvironmentalMap();
        ActivateTexture(TextureSlot::BRDFIntegration);
        environmentalMap->BindBRDFIntegrationMap();
    }
}

void RenderingPipeline::ActivateTexture(TextureSlot textureSlot)
{
    glActiveTexture(GL_TEXTURE0 + static_cast<int>(textureSlot));
}

void RenderingPipeline::ActivateTexture(TextureSlot textureSlot, Texture* textureToBind)
{
    glActiveTexture(GL_TEXTURE0 + static_cast<int>(textureSlot));
    textureToBind->Bind();
}


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
    //for (const auto camera : cameras)
    {
        RenderCamera(mainCamera);
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
        return renderer->ToBeDestroyed();
    });
    std::experimental::erase_if(lights, [](const auto light)
    {
        return light->ToBeDestroyed();
    });
    std::experimental::erase_if(cameras, [](const auto camera)
    {
        return camera->ToBeDestroyed();
    });
    if (mainCamera && mainCamera->ToBeDestroyed())
        mainCamera = nullptr;
}

void RenderingPipeline::SetSkybox(Skybox* skybox_)
{
	skybox = skybox_;
}

void RenderingPipeline::RenderCamera(Camera* camera)
{
    RenderQueue queue;
    FillRenderQueue(&queue, camera);
    camera->BindCamera();

    RenderGBuffer(&queue);
    RenderGlobalIllumination();
    RenderLights();
    BlendIllumination();
    RenderToneMapping();
    if(skybox)
        RenderSkybox(camera);
    RenderToDefaultFrameBuffer(tonemappingTexture.get());
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderingPipeline::RenderGBuffer(RenderQueue* queue)
{
    gbuffer.buffer->Bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);

    for (const auto& renderTarget : queue->queue)
    {
        renderTarget.first->Render(renderTarget.second, ShaderFactory::Type::GBuffer);
    }
}

void RenderingPipeline::RenderGlobalIllumination()
{
    globalIlluminationBuffer->Bind();

    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    RenderingPipeline::BindSkyboxEnvironmentalMap();
    ActivateTexture(TextureSlot::Albedo, gbuffer.colorTexture.get());
    ActivateTexture(TextureSlot::Normal, gbuffer.normalTexture.get());
    ActivateTexture(TextureSlot::Height, gbuffer.depthTexture.get());
    ActivateTexture(TextureSlot::Metallic, gbuffer.metallicTexture.get());

    auto shader = Engine::GetShaderFactory().GetShader(ShaderFactory::Type::PBR_IlluminationGlobal);
    shader->setup();

    quad->Draw(shader);
}

void RenderingPipeline::RenderLights()
{
    lightsIlluminationBuffer->Bind();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    ActivateTexture(TextureSlot::Albedo, gbuffer.colorTexture.get());
    ActivateTexture(TextureSlot::Normal, gbuffer.normalTexture.get());
    ActivateTexture(TextureSlot::Height, gbuffer.depthTexture.get());
    ActivateTexture(TextureSlot::Metallic, gbuffer.metallicTexture.get());

    auto shader = Engine::GetShaderFactory().GetShader(ShaderFactory::Type::PBR_IlluminationLights);
    shader->setup();

    for (const auto& light : lights)
    {
        light->BindLight();
        quad->Draw(shader);
    }
}

void RenderingPipeline::BlendIllumination()
{
    lightsIlluminationBuffer->Bind();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    ActivateTexture(TextureSlot::Environmental, globalIlluminationTexture.get());

    auto shader = Engine::GetShaderFactory().GetShader(ShaderFactory::Type::SimpleCopy);
    shader->setup();
    quad->Draw(shader);
}

void RenderingPipeline::RenderToneMapping()
{
    tonemappingBuffer->Bind();

    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    ActivateTexture(TextureSlot::Environmental, lightsIlluminationTexture.get());

    auto shader = Engine::GetShaderFactory().GetShader(ShaderFactory::Type::ToneMapping);
    shader->setup();
    quad->Draw(shader);
}

void RenderingPipeline::RenderSkybox(Camera* camera)
{
    tonemappingBuffer->Bind();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    RenderingPipeline::BindCameraUniform();
    auto skyboxViewMatrix = glm::mat4(glm::mat3(glm::inverse(camera->GetTransform()->GetModelMatrix())));
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(skyboxViewMatrix), &skyboxViewMatrix);

    skybox->Render();

    glDepthFunc(GL_LESS);
}

void RenderingPipeline::RenderToDefaultFrameBuffer(Texture* texture)
{
    Framebuffer::Unbind();

    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    ActivateTexture(TextureSlot::Environmental, tonemappingTexture.get());

    auto shader = Engine::GetShaderFactory().GetShader(ShaderFactory::Type::SimpleCopy);
    shader->setup();
    quad->Draw(shader);
}

void RenderingPipeline::FillRenderQueue(RenderQueue* queue, Camera* camera)
{
    for (auto renderer : renderers)
    {
        renderer->AddToRenderQueue(queue);
    }
}
