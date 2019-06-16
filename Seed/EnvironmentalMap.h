#pragma once

class TextureCubeMap;
class Texture;

class EnvironmentalMap
{
public:
    EnvironmentalMap();

    void Load(TextureCubeMap* environment);

    void BindIrradiance();
    void BindEnvironmentalMap();
    void BindBRDFIntegrationMap();

private:
    std::unique_ptr<TextureCubeMap> irradiance;
    std::unique_ptr<TextureCubeMap> environmentalMap;
    std::unique_ptr<Texture> BRDFIntegrationMap;
};

