#pragma once
#include "Shader.h"

class ShaderFactory
{
public:
    enum class Type
    {
        Basic,
        Skybox,
        EquirectangularToCubemap,
        PrecalculateIrradiance,
        PrecalculateEnvironmentMap,
        PrecalculateBRDFIntegrationMap
    };

    ShaderFactory();

    Shader* GetShader(Type type);

private:
    void CreateShader(Type type);

    std::unordered_map<Type, std::unique_ptr<Shader>> shaders;
};
