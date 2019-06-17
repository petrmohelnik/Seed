#include "Skybox.h"
#include "Engine.h"
#include "TextureCubeMap.h"
#include "EnvironmentalMap.h"

Skybox::Skybox(std::unique_ptr<TextureCubeMap> skybox_) : skybox(std::move(skybox_)), cube(SimpleMesh::Shape::Cube)
{
    skybox->Load();
    environmentalMap = std::make_unique<EnvironmentalMap>();
    environmentalMap->Load(skybox.get());
}

void Skybox::Render()
{
	glActiveTexture(GL_TEXTURE6);
    skybox->Bind();

    auto shader = Engine::GetShaderFactory().GetShader(ShaderFactory::Type::Skybox);
    shader->setup();

    cube.Bind();

    shader->draw(36);
}

TextureCubeMap* Skybox::GetSkyboxCubeMapRawPtr()
{
    return skybox.get();
}

EnvironmentalMap* Skybox::GetEnvironmentalMapRawPtr()
{
    return environmentalMap.get();
}
