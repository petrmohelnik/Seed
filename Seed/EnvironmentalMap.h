#pragma once

class TextureCubeMap;

class EnvironmentalMap
{
public:
    EnvironmentalMap();

    void Load(TextureCubeMap* environment);

    void BindIrradiance();

private:
    std::unique_ptr<TextureCubeMap> irradiance;
};

