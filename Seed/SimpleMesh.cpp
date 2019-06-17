#include "SimpleMesh.h"

SimpleMesh::SimpleMesh(Shape shape)
{
    MeshData data;

    if (shape == Shape::Quad)
        data = GenerateQuadData();
    else if (shape == Shape::Cube)
        data = GenerateCubeData();

    glGenBuffers(2, &vbo[0]);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(float) * 3, &data.vertices[0].x, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(Uint32) * 3, &data.indices[0].x, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

SimpleMesh::~SimpleMesh()
{
    glDeleteBuffers(2, &vbo[0]);
    glDeleteVertexArrays(1, &vao);
}

void SimpleMesh::Bind()
{
    glBindVertexArray(vao);
}

SimpleMesh::MeshData SimpleMesh::GenerateQuadData()
{
    MeshData data;

    data.vertices =
    {
        glm::vec3(-1.0f,  1.0f, 0.0f),
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f,  1.0f, 0.0f),
    };

    data.indices =
    {
        glm::uvec3(0, 1, 2),
        glm::uvec3(2, 3, 0),
    };

    return data;
}

SimpleMesh::MeshData SimpleMesh::GenerateCubeData()
{
    MeshData data;

    data.vertices =
    {
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f, 1.0f),
        glm::vec3(-1.0f, -1.0f, 1.0f),
        glm::vec3(1.0f, -1.0f, 1.0f),
        glm::vec3(1.0f,  1.0f, 1.0f),
    };

    data.indices =
    {
        glm::uvec3(0, 1, 2),
        glm::uvec3(2, 3, 0),
        glm::uvec3(5, 1, 0),
        glm::uvec3(0, 4, 5),
        glm::uvec3(2, 6, 7),
        glm::uvec3(7, 3, 2),
        glm::uvec3(5, 4, 7),
        glm::uvec3(7, 6, 5),
        glm::uvec3(0, 3, 7),
        glm::uvec3(7, 4, 0),
        glm::uvec3(1, 5, 6),
        glm::uvec3(6, 2, 1)
    };

    return data;
}


