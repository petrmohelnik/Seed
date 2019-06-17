#include "EnvironmentalMap.h"
#include "TextureCubeMap.h"
#include "Texture.h"
#include "Engine.h"

EnvironmentalMap::EnvironmentalMap()
{
    irradiance = std::make_unique<TextureCubeMap>();
    environmentalMap = std::make_unique<TextureCubeMap>();
    BRDFIntegrationMap = std::make_unique<Texture>();
}

void EnvironmentalMap::Load(TextureCubeMap* environment)
{
    irradiance->RenderIntoHDRCubeMapFromTexture(32, ShaderFactory::Type::PrecalculateIrradiance, GL_TEXTURE6, GL_TEXTURE_CUBE_MAP, environment->texture);

    glBindTexture(GL_TEXTURE_CUBE_MAP, environment->texture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    environmentalMap->RenderIntoHDRCubeMapFromTexture(512, ShaderFactory::Type::PrecalculateEnvironmentMap, GL_TEXTURE6, GL_TEXTURE_CUBE_MAP, environment->texture,
        true, 0, 5);

    BRDFIntegrationMap->RenderIntoHDRTexture(1024, 1024, ShaderFactory::Type::PrecalculateBRDFIntegrationMap, GL_RG);
}

void EnvironmentalMap::BindIrradiance()
{
    irradiance->Bind();
}

void EnvironmentalMap::BindEnvironmentalMap()
{
    environmentalMap->Bind();
}

void EnvironmentalMap::BindBRDFIntegrationMap()
{
    BRDFIntegrationMap->Bind();
}
