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
    boundingSphere = std::make_unique<SimpleMesh>(SimpleMesh::Shape::BoundingSphere);

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
    gbuffer.colorTexture->GenerateTexture(GL_CLAMP_TO_EDGE, GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, false);
    gbuffer.normalTexture = std::make_unique<Texture>();
    gbuffer.normalTexture->GenerateTexture(GL_CLAMP_TO_EDGE, GL_RGBA16F, width, height, GL_RGBA, GL_FLOAT, false);
    gbuffer.metallicTexture = std::make_unique<Texture>();
    gbuffer.metallicTexture->GenerateTexture(GL_CLAMP_TO_EDGE, GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, false);
    gbuffer.depthStencilTexture = std::make_unique<Texture>();
    gbuffer.depthStencilTexture->GenerateTexture(GL_CLAMP_TO_EDGE, GL_DEPTH_STENCIL, width, height, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, false);

    shadowMapTexture = std::make_unique<Texture>();
    shadowMapTexture->GenerateTexture(GL_CLAMP_TO_BORDER, GL_DEPTH_COMPONENT, 2048, 2048, GL_DEPTH_COMPONENT, GL_FLOAT, false);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture->texture);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    lightsIlluminationTexture = std::make_unique<Texture>();
    lightsIlluminationTexture->GenerateTexture(GL_CLAMP_TO_EDGE, GL_RGB16F, width, height, GL_RGB, GL_FLOAT, false);

    globalIlluminationTexture = std::make_unique<Texture>();
    globalIlluminationTexture->GenerateTexture(GL_CLAMP_TO_EDGE, GL_RGB16F, width, height, GL_RGB, GL_FLOAT, false);

    tonemappingTexture = std::make_unique<Texture>();
    tonemappingTexture->GenerateTexture(GL_CLAMP_TO_EDGE, GL_RGB8, width, height, GL_RGB, GL_UNSIGNED_BYTE, false);
}

void RenderingPipeline::IntializeBuffers(int width, int height)
{
    gbuffer.buffer = std::make_unique<Framebuffer>(width, height);
    gbuffer.buffer->AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gbuffer.colorTexture->texture);
    gbuffer.buffer->AttachTexture(GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gbuffer.normalTexture->texture);
    gbuffer.buffer->AttachTexture(GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gbuffer.metallicTexture->texture);
    gbuffer.buffer->AttachTexture(GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, lightsIlluminationTexture->texture);
    gbuffer.buffer->AttachTexture(GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gbuffer.depthStencilTexture->texture);
    gbuffer.buffer->SetDrawBuffers({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 });

    shadowMapBuffer = std::make_unique<Framebuffer>(2048, 2048);
    shadowMapBuffer->SetNoColorAttachment();
    shadowMapBuffer->AttachTexture(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture->texture);

    lightsIlluminationBuffer = std::make_unique<Framebuffer>(width, height);
    lightsIlluminationBuffer->AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightsIlluminationTexture->texture);
    lightsIlluminationBuffer->AttachTexture(GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gbuffer.depthStencilTexture->texture);

    globalIlluminationBuffer = std::make_unique<Framebuffer>(width, height);
    globalIlluminationBuffer->AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, globalIlluminationTexture->texture);

    tonemappingBuffer = std::make_unique<Framebuffer>(width, height);
    tonemappingBuffer->AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tonemappingTexture->texture);
    tonemappingBuffer->AttachTexture(GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gbuffer.depthStencilTexture->texture);
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
        RenderCamera(*mainCamera);
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

void RenderingPipeline::RenderCamera(Camera& camera)
{
    camera.BindCamera();
    camera.UpdateFrustum();
    RenderQueue deferredQueue(&camera);
    RenderQueue forwardQueue(&camera);
    FillRenderQueues(deferredQueue, forwardQueue, camera);

    RenderGBuffer(deferredQueue);
    RenderGlobalIllumination();
    RenderLights(camera);
    BlendIllumination();
    RenderToneMapping();
    RenderForward(forwardQueue);
    if(skybox)
        RenderSkybox(camera);
    RenderToDefaultFrameBuffer(*tonemappingTexture.get());
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderingPipeline::RenderGBuffer(RenderQueue& queue)
{
    gbuffer.buffer->Bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);

    for (const auto& renderTarget : queue.queue)
    {
        renderTarget.meshRenderer->Render(renderTarget.submeshIndex);
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
    ActivateTexture(TextureSlot::Height, gbuffer.depthStencilTexture.get());
    ActivateTexture(TextureSlot::Metallic, gbuffer.metallicTexture.get());

    auto shader = Engine::GetShaderFactory().GetShader(ShaderFactory::Type::PBR_IlluminationGlobal);
    shader->setup();
    shader->SetUniformIVec2("screenSize", globalIlluminationBuffer->GetSize());

    quad->Draw(shader);
}

void RenderingPipeline::RenderShadowMap(const Light& light)
{
    shadowMapBuffer->Bind();

    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);

    glm::mat4 lightProjection = glm::perspective(light.outerAngle, 1.0f, 0.2f, light.dataBlock.Range == 0.0f ? 1000.0f : light.dataBlock.Range);

    glm::mat4 lightView = glm::lookAt(light.dataBlock.Pos,
        light.dataBlock.Pos + light.dataBlock.Orientation,
        glm::vec3(0.0f, 1.0f, 0.0f));

    RenderingPipeline::BindCameraUniform();
    Camera::CameraBlock dataBlock;
    dataBlock.projection = lightProjection;
    dataBlock.view = lightView;
    dataBlock.viewPos = light.dataBlock.Pos;
    glBufferData(GL_UNIFORM_BUFFER, sizeof(dataBlock), &dataBlock, GL_DYNAMIC_DRAW);

    RenderQueue shadowRenderQueue;
    for (auto renderer : renderers)
    {
        //if (glm::distance(renderer->GetTransform()->GetPosition(), light.dataBlock.Pos) < light.dataBlock.Range)
        {
            renderer->AddToRenderQueueDeferred(shadowRenderQueue);
            renderer->AddToRenderQueueForward(shadowRenderQueue);
        }
    }

    for (const auto& renderTarget : shadowRenderQueue.queue)
    {
        renderTarget.meshRenderer->Render(renderTarget.submeshIndex, ShaderFactory::Type::SimplePositionModel);
    }
    
    glCullFace(GL_BACK);
}

void RenderingPipeline::RenderLights(Camera& camera)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    ActivateTexture(TextureSlot::Albedo, gbuffer.colorTexture.get());
    ActivateTexture(TextureSlot::Normal, gbuffer.normalTexture.get());
    ActivateTexture(TextureSlot::Height, gbuffer.depthStencilTexture.get());
    ActivateTexture(TextureSlot::Metallic, gbuffer.metallicTexture.get());

    auto shaderStencil = Engine::GetShaderFactory().GetShader(ShaderFactory::Type::SimplePositionModel);
    auto shaderLights = Engine::GetShaderFactory().GetShader(ShaderFactory::Type::PBR_IlluminationLightsSphere);
    auto shaderLightsQuad = Engine::GetShaderFactory().GetShader(ShaderFactory::Type::PBR_IlluminationLightsQuad);

    for (const auto& light : lights)
    {
        light->BindLight();
        RenderShadowMap(*light);
        lightsIlluminationBuffer->Bind();
        camera.BindCamera();

        ActivateTexture(TextureSlot::Shadow, shadowMapTexture.get());

        glm::mat4 lightProjection = glm::perspective(light->outerAngle, 1.0f, 0.2f, light->dataBlock.Range == 0.0f ? 1000.0f : light->dataBlock.Range);
        glm::mat4 lightView = glm::lookAt(light->dataBlock.Pos,
            light->dataBlock.Pos + light->dataBlock.Orientation,
            glm::vec3(0.0f, 1.0f, 0.0f));

        light->dataBlock.LightSpaceMatrix = lightProjection * lightView;

        if (light->dataBlock.Range == 0.0f)
        {
            glDisable(GL_DEPTH_TEST);
            shaderLightsQuad->setup();
            shaderLightsQuad->SetUniformIVec2("screenSize", lightsIlluminationBuffer->GetSize());
            quad->Draw(shaderLightsQuad);
            continue;
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glDepthMask(GL_FALSE);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0x00);

        auto modelMatrix = glm::translate(light->dataBlock.Pos) * glm::scale(glm::vec3(light->dataBlock.Range));
        RenderingPipeline::BindModelUniform();
        MeshRenderer::ModelBlock modelBlockData
        {
            modelMatrix,
            glm::mat4(glm::inverse(modelMatrix))
        };
        glBufferData(GL_UNIFORM_BUFFER, sizeof(modelBlockData), &modelBlockData, GL_DYNAMIC_DRAW);
        shaderStencil->setup();
        boundingSphere->Draw(shaderStencil);

        glCullFace(GL_FRONT);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthFunc(GL_GEQUAL);
        glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
        glStencilFunc(GL_EQUAL, 0, 0xFF);
        glStencilMask(0xFF);

        shaderLights->setup();
        shaderLights->SetUniformIVec2("screenSize", lightsIlluminationBuffer->GetSize());
        boundingSphere->Draw(shaderLights);
    
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glDisable(GL_STENCIL_TEST);
    }
    glCullFace(GL_BACK);
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

void RenderingPipeline::RenderForward(RenderQueue& queue)
{
    tonemappingBuffer->Bind();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);

    for (const auto& renderTarget : queue.queue)
    {
        renderTarget.meshRenderer->Render(renderTarget.submeshIndex);
    }
}

void RenderingPipeline::RenderSkybox(Camera& camera)
{
    tonemappingBuffer->Bind();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    RenderingPipeline::BindCameraUniform();
    auto skyboxViewMatrix = glm::mat4(glm::mat3(glm::inverse(camera.GetTransform()->GetModelMatrix())));
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(skyboxViewMatrix), &skyboxViewMatrix);

    skybox->Render();

    glDepthFunc(GL_LESS);
}

void RenderingPipeline::RenderToDefaultFrameBuffer(Texture& texture)
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

void RenderingPipeline::FillRenderQueues(RenderQueue& deferredQueue, RenderQueue& forwardQueue, const Camera& camera)
{
    for (auto renderer : renderers)
    {
        renderer->AddToRenderQueueDeferred(deferredQueue);
        renderer->AddToRenderQueueForward(forwardQueue);
    }
}
