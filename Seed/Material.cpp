#include "Material.h"
#include "RenderingPipeline.h"

Material::Material()
{
    Albedo = std::make_shared<Texture>(glm::vec4(1.0f));
    Albedo->SetIsSRGB();
    Metallic = std::make_shared<Texture>(glm::vec4(1.0f));
    Normal = std::make_shared<Texture>(glm::vec4(0.5f, 0.5f, 1.0f, 0.0f));
    Emission = std::make_shared<Texture>(glm::vec4(0.0f));
    Height = std::make_shared<Texture>(glm::vec4(1.0f));
    Occlusion = std::make_shared<Texture>(glm::vec4(1.0f));
}

Material::~Material()
{
}

void Material::Load()
{
    Albedo->Load();
    Normal->Load();
    Height->Load();
    Metallic->Load();
    Emission->Load();
    Occlusion->Load();
}

void Material::Unload()
{
    Albedo->Unload();
    Normal->Unload();
    Height->Unload();
    Metallic->Unload();
    Emission->Unload();
    Occlusion->Unload();
}

void Material::BindMaterial()
{
    RenderingPipeline::BindMaterialUniform();
    glBufferData(GL_UNIFORM_BUFFER, sizeof(dataBlock), &dataBlock, GL_DYNAMIC_DRAW);

    RenderingPipeline::ActivateTexture(RenderingPipeline::TextureSlot::Albedo, Albedo.get());
    RenderingPipeline::ActivateTexture(RenderingPipeline::TextureSlot::Normal, Normal.get());
    RenderingPipeline::ActivateTexture(RenderingPipeline::TextureSlot::Height, Height.get());
    RenderingPipeline::ActivateTexture(RenderingPipeline::TextureSlot::Metallic, Metallic.get());
    RenderingPipeline::ActivateTexture(RenderingPipeline::TextureSlot::Emission, Emission.get());
    RenderingPipeline::ActivateTexture(RenderingPipeline::TextureSlot::Occlusion, Occlusion.get());
}

void Material::SetShader(ShaderFactory::Type shader_)
{
    shader = shader_;
}

ShaderFactory::Type Material::GetShader()
{
    return shader;
}

void Material::SetSpecularWorkflow()
{
    dataBlock.SpecularWorkflow = true;
}

void Material::SetMetallicWorkflow()
{
    dataBlock.SpecularWorkflow = false;
}

void Material::UseOcclusionMap()
{
    dataBlock.UseOcclusionMap = true;
}

void Material::SetParallaxStrength(float strength)
{
    dataBlock.ParallaxStrength = strength;
}

std::shared_ptr<Material> Material::Copy()
{
    auto clonedMaterial = std::make_shared<Material>();

    clonedMaterial->Albedo = Albedo->Copy();
    clonedMaterial->Normal = Normal->Copy();
    clonedMaterial->Height = Height->Copy();
    clonedMaterial->Metallic = Metallic->Copy();
    clonedMaterial->Emission = Emission->Copy();
    clonedMaterial->Occlusion = Occlusion->Copy();
    clonedMaterial->dataBlock = dataBlock;
    clonedMaterial->shader = shader;

    return clonedMaterial;
}
