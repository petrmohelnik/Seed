#pragma once
#include "Shader.h"

class ShaderFactory
{
public:
    enum class Type
    {
        GBuffer,
        PBR_IlluminationGlobal,
        SimplePositionModel,
        PBR_IlluminationLightsQuad,
        PBR_IlluminationLightsSphere,
        Skybox,
        EquirectangularToCubemap,
        PrecalculateIrradiance,
        PrecalculateEnvironmentMap,
        PrecalculateBRDFIntegrationMap,
        ToneMapping,
        SkyboxToneMapping,
        SimpleCopy,
        PointLightShadow
    };

    ShaderFactory();

    Shader* GetShader(Type type);

private:
    void CreateShader(Type type);

    std::unordered_map<Type, std::unique_ptr<Shader>> shaders;
};
