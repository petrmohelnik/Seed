#pragma once
#include "EnvironmentalMap.h"
#include "SimpleMesh.h"

class TextureCubeMap;
class EnvironmentalMap;

class Skybox
{
public:
    Skybox(std::unique_ptr<TextureCubeMap> cubeMap);

protected:
    friend class RenderingPipeline;

    void Render();
    TextureCubeMap* GetSkyboxCubeMapRawPtr();
    EnvironmentalMap* GetEnvironmentalMapRawPtr();

private:
    std::unique_ptr<TextureCubeMap> skybox;
    std::unique_ptr<EnvironmentalMap> environmentalMap;

    SimpleMesh cube;
};

