#include "Material.h"
#include "RenderingPipeline.h"

Material::Material()
{
    Diffuse = std::make_shared<Texture>(glm::vec4(1.0f));
    Specular = std::make_shared<Texture>(glm::vec4(0.0f));
    Normal = std::make_shared<Texture>(glm::vec4(0.5f, 0.5f, 1.0f, 0.0f));
    Emission = std::make_shared<Texture>(glm::vec4(0.0f));
    Height = std::make_shared<Texture>(glm::vec4(0.0f));
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
    Diffuse->Bind();
    glActiveTexture(GL_TEXTURE1);
    Normal->Bind();
    glActiveTexture(GL_TEXTURE2);
    Height->Bind();
    glActiveTexture(GL_TEXTURE3);
    Specular->Bind();
    glActiveTexture(GL_TEXTURE4);
    Emission->Bind();

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
