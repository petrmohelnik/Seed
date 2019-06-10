#include "Skybox.h"
#include "TextureCubeMap.h"
#include "Engine.h"
#include "EnvironmentalMap.h"

Skybox::Skybox(std::unique_ptr<TextureCubeMap> skybox_) : skybox(std::move(skybox_))
{
    skybox->Load();
    skybox->LoadCubeMesh(&vao, &vbo[0]);
    environmentalMap = std::make_unique<EnvironmentalMap>(skybox.get());
}

Skybox::~Skybox()
{
    glDeleteBuffers(2, &vbo[0]);
    glDeleteVertexArrays(1, &vao);
}

void Skybox::Render()
{
	glActiveTexture(GL_TEXTURE5);
    skybox->BindTexture();
    auto shader = Engine::GetShaderFactory().GetShader(ShaderFactory::Type::Skybox);
    shader->setup();

    glBindVertexArray(vao);

    shader->draw(36);
}

TextureCubeMap * Skybox::GetCubeMapRawPtr()
{
    return skybox.get();
}
