#pragma once

class TextureCubeMap;

class Skybox
{
public:
    Skybox(std::shared_ptr<TextureCubeMap> cubeMap);
    ~Skybox();

    std::shared_ptr<TextureCubeMap> GetCubeMap();
    std::shared_ptr<TextureCubeMap> GetSharedCubeMap();

    void Render();

private:
    void LoadCubeMesh();

    std::shared_ptr<TextureCubeMap> cubeMap;

    GLuint vao = 0;
    GLuint vbo[2];
};

