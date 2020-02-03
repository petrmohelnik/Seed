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
    irradiance->RenderIntoHDRCubeMapFromTexture(32, ShaderFactory::Type::PrecalculateIrradiance, RenderingPipeline::TextureSlot::Environmental, GL_TEXTURE_CUBE_MAP, environment->texture);

    environmentalMap->RenderIntoHDRCubeMapFromTexture(512, ShaderFactory::Type::PrecalculateEnvironmentMap, RenderingPipeline::TextureSlot::Environmental, GL_TEXTURE_CUBE_MAP, environment->texture,
        true, true, 5);

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
