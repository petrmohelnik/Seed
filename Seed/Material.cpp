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

    glActiveTexture(GL_TEXTURE0);
    Albedo->Bind();
    glActiveTexture(GL_TEXTURE1);
    Normal->Bind();
    glActiveTexture(GL_TEXTURE2);
    Height->Bind();
    glActiveTexture(GL_TEXTURE3);
    Metallic->Bind();
    glActiveTexture(GL_TEXTURE4);
    Emission->Bind();
    glActiveTexture(GL_TEXTURE5);
    Occlusion->Bind();

    RenderingPipeline::BindSkyboxEnvironmentalMap();
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
