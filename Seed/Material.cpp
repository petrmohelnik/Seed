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
    diffuse.Load();
    specular.Load();
    normal.Load();
    height.Load();
}

void Material::Unload()
{
    diffuse.Unload();
    specular.Unload();
    normal.Unload();
    height.Unload();
}

void Material::BindMaterial()
{
    RenderingPipeline::BindMaterialUniform();
    MaterialBlock materialBlockData{ glm::vec4() };
    glBufferData(GL_UNIFORM_BUFFER, sizeof(materialBlockData), &materialBlockData, GL_DYNAMIC_DRAW);

    glActiveTexture(GL_TEXTURE0);
    diffuse.BindTexture();
    glActiveTexture(GL_TEXTURE1);
    normal.BindTexture();
    glActiveTexture(GL_TEXTURE2);
    specular.BindTexture();
}

void Material::SetShader(ShaderFactory::Type shader_)
{
    shader = shader_;
}

ShaderFactory::Type Material::GetShader()
{
    return shader;
}
