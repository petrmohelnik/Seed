#include "Skybox.h"
#include "TextureCubeMap.h"
#include "Engine.h"

Skybox::Skybox(std::shared_ptr<TextureCubeMap> cubeMap) : cubeMap(cubeMap)
{
    cubeMap->Load();
    LoadCubeMesh();
}

void Skybox::LoadCubeMesh()
{
    std::vector<float> skyboxVertices = 
    {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    glGenBuffers(2, &vbo[0]);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), &skyboxVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Skybox::~Skybox()
{
    glDeleteBuffers(1, &vbo[0]);
    glDeleteVertexArrays(1, &vao);
}

std::shared_ptr<TextureCubeMap> Skybox::GetCubeMap()
{
    cubeMap = std::make_shared<TextureCubeMap>(*cubeMap);
    return cubeMap;
}

std::shared_ptr<TextureCubeMap> Skybox::GetSharedCubeMap()
{
    return cubeMap;
}

void Skybox::Render()
{
    cubeMap->BindTexture();
    auto shader = Engine::GetShaderFactory().GetShader(ShaderFactory::Type::Skybox);
    shader->setup();

    glBindVertexArray(vao);

    //shader->draw(mesh->SubmeshIndicesCount(index)); ???
}
