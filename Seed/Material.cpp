#include "Material.h"
#include "RenderingPipeline.h"

Material::Material()
{
    Diffuse = std::make_shared<Texture>();
    Specular = std::make_shared<Texture>();
    Normal = std::make_shared<Texture>();
    Emission = std::make_shared<Texture>();
    Height = std::make_shared<Texture>();
}

Material::~Material()
{
}

void Material::Load()
{
    Diffuse->Load();
    Normal->Load();
    Height->Load();
    Specular->Load();
    Emission->Load();
}

void Material::Unload()
{
    Diffuse->Unload();
    Normal->Unload();
    Height->Unload();
    Specular->Unload();
    Emission->Unload();
}

void Material::BindMaterial()
{
    RenderingPipeline::BindMaterialUniform();
    glBufferData(GL_UNIFORM_BUFFER, sizeof(dataBlock), &dataBlock, GL_DYNAMIC_DRAW);

    glActiveTexture(GL_TEXTURE0);
    Diffuse->BindTexture();
    glActiveTexture(GL_TEXTURE1);
    Normal->BindTexture();
    glActiveTexture(GL_TEXTURE2);
    Height->BindTexture();
    glActiveTexture(GL_TEXTURE3);
    Specular->BindTexture();
    glActiveTexture(GL_TEXTURE4);
    Emission->BindTexture();

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

void Material::SetMetallic()
{
	dataBlock.isMetallic = true;
}
