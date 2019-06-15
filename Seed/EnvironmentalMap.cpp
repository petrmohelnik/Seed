#include "EnvironmentalMap.h"
#include "TextureCubeMap.h"
#include "Engine.h"

EnvironmentalMap::EnvironmentalMap()
{
    irradiance = std::make_unique<TextureCubeMap>();
}

void EnvironmentalMap::Load(TextureCubeMap* environment)
{
    irradiance->GenerateOpenglTexture();
    irradiance->DefineOpenglTexture(GL_RGB16F, 32, 32, GL_RGB, GL_FLOAT);

    GLuint vao, vbo[2];
    TextureCubeMap::LoadCubeMesh(&vao, &vbo[0]);

    GLuint fbo, rbo;
    TextureCubeMap::GenerateFrameBuffer(&fbo, &rbo, 32, 32);

    auto shader = Engine::GetShaderFactory().GetShader(ShaderFactory::Type::PrecalculateIrradiance);
    shader->setup();

    Camera::CameraBlock camera;
    camera.projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    auto const captureViews = TextureCubeMap::GenerateCameraViewsForCube();

    glActiveTexture(GL_TEXTURE5);
    environment->BindTexture();

    glViewport(0, 0, 32, 32);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    for (unsigned int i = 0; i < 6; i++)
    {
        camera.view = captureViews[i];
        RenderingPipeline::BindCameraUniform();
        glBufferData(GL_UNIFORM_BUFFER, sizeof(camera), &camera, GL_DYNAMIC_DRAW);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance->texture, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);
        shader->draw(36);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    TextureCubeMap::UnloadCubeMesh(&vao, &vbo[0]);
    TextureCubeMap::DeleteFrameBuffer(&fbo, &rbo);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    Engine::GetWindow().ResetViewport();
}

void EnvironmentalMap::BindIrradiance()
{
    irradiance->BindTexture();
}
