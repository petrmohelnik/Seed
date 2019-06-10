#pragma once

class TextureCubeMap;
class EnvironmentalMap;

class Skybox
{
public:
    Skybox(std::unique_ptr<TextureCubeMap> cubeMap);
    ~Skybox();

protected:
    friend class RenderingPipeline;

    void Render();
    TextureCubeMap* GetCubeMapRawPtr();

private:
    std::unique_ptr<TextureCubeMap> skybox;
    std::unique_ptr<EnvironmentalMap> environmentalMap;

    GLuint vao = 0;
    GLuint vbo[2];
};

