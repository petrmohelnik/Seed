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
        shaders.insert({ type , std::make_unique<BasicShader>(ShaderCompiler::Compile("GBuffer.vert", "GBuffer.frag")) });
    }
    else if (type == Type::PBR_IlluminationGlobal)
    {
        shaders.insert({ type , std::make_unique<BasicShader>(ShaderCompiler::Compile("PBR_IlluminationQuad.vert", "PBR_IlluminationGlobal.frag")) });
    }
    else if (type == Type::SimplePositionModel)
    {
        shaders.insert({ type , std::make_unique<BasicShader>(ShaderCompiler::Compile("SimplePositionModel.vert", "Dummy.frag")) });
    }
    else if (type == Type::PBR_IlluminationLightsQuad)
    {
        shaders.insert({ type , std::make_unique<BasicShader>(ShaderCompiler::Compile("PBR_IlluminationQuad.vert", "PBR_IlluminationLights.frag")) });
    }
    else if (type == Type::PBR_IlluminationLightsSphere)
    {
        shaders.insert({ type , std::make_unique<BasicShader>(ShaderCompiler::Compile("PBR_IlluminationSphere.vert", "PBR_IlluminationLights.frag")) });
    }
    else if (type == Type::Skybox)
    {
        shaders.insert({ type , std::make_unique<BasicShader>(ShaderCompiler::Compile("Skybox.vert", "Skybox.frag")) });
    }
    else if (type == Type::EquirectangularToCubemap)
    {
        shaders.insert({ type , std::make_unique<BasicShader>(ShaderCompiler::Compile("SimplePosition.vert", "EquirectangularToCubemap.frag")) });
    }
    else if (type == Type::PrecalculateIrradiance)
    {
        shaders.insert({ type , std::make_unique<BasicShader>(ShaderCompiler::Compile("SimplePosition.vert", "PrecalculateIrradiance.frag")) });
    }
    else if (type == Type::PrecalculateEnvironmentMap)
    {
        shaders.insert({ type , std::make_unique<BasicShader>(ShaderCompiler::Compile("SimplePosition.vert", "PrecalculateEnvironmentMap.frag")) });
    }
    else if (type == Type::PrecalculateBRDFIntegrationMap)
    {
        shaders.insert({ type , std::make_unique<BasicShader>(ShaderCompiler::Compile("SimplePosition2D.vert", "PrecalculateBRDFIntegrationMap.frag")) });
    }
    else if (type == Type::ToneMapping)
    {
        shaders.insert({ type , std::make_unique<BasicShader>(ShaderCompiler::Compile("SimplePosition2D.vert", "ToneMapping.frag")) });
    }
    else if (type == Type::SkyboxToneMapping)
    {
        shaders.insert({ type , std::make_unique<BasicShader>(ShaderCompiler::Compile("SimplePosition.vert", "ToneMappingSkybox.frag")) });
    }
    else if (type == Type::SimpleCopy)
    {
        shaders.insert({ type , std::make_unique<BasicShader>(ShaderCompiler::Compile("SimplePosition2D.vert", "SimpleCopy.frag")) });
    }
    else if (type == Type::PointLightShadow)
    {
        shaders.insert({ type , std::make_unique<BasicShader>(ShaderCompiler::Compile("SimplePositionWorld.vert", "PointLightShadow.geom", "PointLightShadow.frag")) });
    }
    else
    {
        throw std::runtime_error("Unsupported shader type");
    }
}
