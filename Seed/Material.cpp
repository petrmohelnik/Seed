#include "Material.h"
#include "RenderingPipeline.h"

Material::Material()
{
}

Material::~Material()
{
    Unload();
}

void Material::Load()
{
    Diffuse.Load();
    Specular.Load();
    Normal.Load();
    Emission.Load();
    Height.Load();
}

void Material::Unload()
{
    Diffuse.Unload();
    Specular.Unload();
    Normal.Unload();
    Emission.Unload();
    Height.Unload();
}

void Material::BindMaterial()
{
    RenderingPipeline::BindMaterialUniform();
    MaterialBlock materialBlockData{ glm::vec4() };
    glBufferData(GL_UNIFORM_BUFFER, sizeof(materialBlockData), &materialBlockData, GL_DYNAMIC_DRAW);

    glActiveTexture(GL_TEXTURE0);
    Diffuse.BindTexture();
    glActiveTexture(GL_TEXTURE1);
    Normal.BindTexture();
    glActiveTexture(GL_TEXTURE2);
    Specular.BindTexture();
    glActiveTexture(GL_TEXTURE3);
    Emission.BindTexture();
}

void Material::SetShader(ShaderFactory::Type shader_)
{
    shader = shader_;
}

ShaderFactory::Type Material::GetShader()
{
    return shader;
}
