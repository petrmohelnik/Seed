#include "RenderingPipeline.h"
#include "RenderQueue.h"
#include "Renderer.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "Skybox.h"
#include "TextureCubeMap.h"
#include "EnvironmentalMap.h"

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

        glActiveTexture(GL_TEXTURE5);
        environmentalMap->BindIrradiance();
        glActiveTexture(GL_TEXTURE6);
        environmentalMap->BindEnvironmentalMap();
        glActiveTexture(GL_TEXTURE7);
        environmentalMap->BindBRDFIntegrationMap();
    }
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    camera->BindCamera();
    lights[0]->BindLight();
    for (const auto& renderTarget : queue.queue)
    {
        renderTarget.first->Render(renderTarget.second);
    }

	if (skybox)
	{
		RenderingPipeline::BindCameraUniform();
		auto skyboxViewMatrix = glm::mat4(glm::mat3(glm::inverse(camera->GetTransform()->GetModelMatrix())));
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(skyboxViewMatrix), &skyboxViewMatrix);
		glDepthFunc(GL_LEQUAL);
		skybox->Render();
		glDepthFunc(GL_LESS);
	}
}

void RenderingPipeline::FillRenderQueue(RenderQueue* queue, Camera* camera)
{
    for (auto renderer : renderers)
    {
        renderer->AddToRenderQueue(queue);
    }
}
