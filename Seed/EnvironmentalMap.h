#pragma once

class TextureCubeMap;

class EnvironmentalMap
{
public:
    EnvironmentalMap(TextureCubeMap* environment);
    ~EnvironmentalMap();

private:
    std::unique_ptr<TextureCubeMap> irradiance;
};

