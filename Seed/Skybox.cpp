#include "Skybox.h"
#include "Engine.h"
#include "TextureCubeMap.h"
#include "EnvironmentalMap.h"

Skybox::Skybox(std::unique_ptr<TextureCubeMap> skybox_) : skybox(std::move(skybox_)), cube(SimpleMesh::Shape::Cube)
{
    skybox->Load();
    environmentalMap = std::make_unique<EnvironmentalMap>();
    environmentalMap->Load(skybox.get());
    skybox->ConvertFromHDRToSRGB();
}

void Skybox::Render()
{
    RenderingPipeline::ActivateTexture(RenderingPipeline::TextureSlot::Environmental);
    skybox->Bind();

    auto shader = Engine::GetShaderFactory().GetShader(ShaderFactory::Type::Skybox);
    shader->setup();

    cube.Draw(shader);
}

TextureCubeMap* Skybox::GetSkyboxCubeMapRawPtr()
{
    return skybox.get();
}

EnvironmentalMap* Skybox::GetEnvironmentalMapRawPtr()
{
    return environmentalMap.get();
}
