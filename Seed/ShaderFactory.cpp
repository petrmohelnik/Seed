#include "ShaderFactory.h"
#include "BasicShader.h"

ShaderFactory::ShaderFactory()
{
}

Shader* ShaderFactory::GetShader(Type type)
{
    if (shaders.find(type) == shaders.end())
    {
        CreateShader(type);
    }

    return shaders[type].get();
}

void ShaderFactory::CreateShader(Type type)
{
    if (type == Type::GBuffer)
    {
        shaders.insert({ type , std::make_unique<BasicShader>("GBuffer.vert", "GBuffer.frag") });
    }
    else if (type == Type::PBR)
    {
        shaders.insert({ type , std::make_unique<BasicShader>("PBR.vert", "PBR.frag") });
    }
    else if (type == Type::Skybox)
    {
        shaders.insert({ type , std::make_unique<BasicShader>("Skybox.vert", "Skybox.frag") });
    }
    else if (type == Type::EquirectangularToCubemap)
    {
        shaders.insert({ type , std::make_unique<BasicShader>("SimplePosition.vert", "EquirectangularToCubemap.frag") });
    }
    else if (type == Type::PrecalculateIrradiance)
    {
        shaders.insert({ type , std::make_unique<BasicShader>("SimplePosition.vert", "PrecalculateIrradiance.frag") });
    }
    else if (type == Type::PrecalculateEnvironmentMap)
    {
        shaders.insert({ type , std::make_unique<BasicShader>("SimplePosition.vert", "PrecalculateEnvironmentMap.frag") });
    }
    else if (type == Type::PrecalculateBRDFIntegrationMap)
    {
        shaders.insert({ type , std::make_unique<BasicShader>("SimplePosition2D.vert", "PrecalculateBRDFIntegrationMap.frag") });
    }
    else if (type == Type::ToneMapping)
    {
        shaders.insert({ type , std::make_unique<BasicShader>("SimplePosition2D.vert", "ToneMapping.frag") });
    }
    else if (type == Type::SkyboxToneMapping)
    {
        shaders.insert({ type , std::make_unique<BasicShader>("SimplePosition.vert", "ToneMappingSkybox.frag") });
    }
    else if (type == Type::SimpleCopy)
    {
        shaders.insert({ type , std::make_unique<BasicShader>("SimplePosition2D.vert", "SimpleCopy.frag") });
    }
    else
    {
        throw std::runtime_error("Unsupported shader type");
    }
}
