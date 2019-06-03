#pragma once

class TextureCubeMap;

class Skybox
{
public:
    Skybox(std::shared_ptr<TextureCubeMap> cubeMap);
    ~Skybox();

    std::shared_ptr<TextureCubeMap> GetCubeMap();
    std::shared_ptr<TextureCubeMap> GetSharedCubeMap();

protected:
    friend class RenderingPipeline;

    void Render();
    TextureCubeMap* GetCubeMapRawPtr();

private:
    void LoadCubeMesh();

    std::shared_ptr<TextureCubeMap> cubeMap;

    GLuint vao = 0;
    GLuint vbo[2];
};

