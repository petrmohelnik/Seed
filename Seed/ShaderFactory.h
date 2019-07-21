#pragma once
#include "Shader.h"

class ShaderFactory
{
public:
    enum class Type
    {
        GBuffer,
        PBR,
        Skybox,
        EquirectangularToCubemap,
        PrecalculateIrradiance,
        PrecalculateEnvironmentMap,
        PrecalculateBRDFIntegrationMap,
        ToneMapping,
        SkyboxToneMapping,
        SimpleCopy
    };

    ShaderFactory();

    Shader* GetShader(Type type);

private:
    void CreateShader(Type type);

    std::unordered_map<Type, std::unique_ptr<Shader>> shaders;
};
