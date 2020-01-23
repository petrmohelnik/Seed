#include "SimpleMesh.h"
#include "Shader.h"

SimpleMesh::SimpleMesh(Shape shape)
{
    MeshData data;

    if(shape == Shape::Quad)
        data = GenerateQuadData();
    else if(shape == Shape::Cube)
        data = GenerateCubeData();
    else if(shape == Shape::BoundingSphere)
        data = GenerateBoundingSphereData();

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

    indicesCount = static_cast<int>(data.indices.size()) * 3;
}

SimpleMesh::~SimpleMesh()
{
    glDeleteBuffers(2, &vbo[0]);
    glDeleteVertexArrays(1, &vao);
}

void SimpleMesh::Draw(Shader* shader)
{
    glBindVertexArray(vao);
    shader->draw(indicesCount);
}

SimpleMesh::MeshData SimpleMesh::GenerateQuadData()
{
    MeshData data;

    data.vertices =
    {
        glm::vec3(-1.0f,  1.0f, 0.0f),
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f,  1.0f, 0.0f)
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
        glm::vec3(1.0f,  1.0f, 1.0f)
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

SimpleMesh::MeshData SimpleMesh::GenerateBoundingSphereData()
{
    MeshData data;

    data.vertices =
    {
        glm::vec3(0.000000, -1.260000, 0.000000),
        glm::vec3(0.911736, -0.563491, 0.662407),
        glm::vec3(-0.348245, -0.563491, 1.071806),
        glm::vec3(-1.126975, -0.563491, 0.000000),
        glm::vec3(-0.348245, -0.563491, -1.071806),
        glm::vec3(0.911736, -0.563491, -0.662407),
        glm::vec3(0.348245, 0.563491, 1.071806),
        glm::vec3(-0.911736, 0.563491, 0.662407),
        glm::vec3(-0.911736, 0.563491, -0.662407),
        glm::vec3(0.348245, 0.563491, -1.071806),
        glm::vec3(1.126975, 0.563491, 0.000000),
        glm::vec3(0.000000, 1.260000, 0.000000)
    };

    data.indices =
    {
        glm::uvec3(0, 1, 2),
        glm::uvec3(1, 0, 5),
        glm::uvec3(0, 2, 3),
        glm::uvec3(0, 3, 4),
        glm::uvec3(0, 4, 5),
        glm::uvec3(1, 5, 10),
        glm::uvec3(2, 1, 6),
        glm::uvec3(3, 2, 7),
        glm::uvec3(4, 3, 8),
        glm::uvec3(5, 4, 9),
        glm::uvec3(1, 10, 6),
        glm::uvec3(2, 6, 7),
        glm::uvec3(3, 7, 8),
        glm::uvec3(4, 8, 9),
        glm::uvec3(5, 9, 10),
        glm::uvec3(6, 10, 11),
        glm::uvec3(7, 6, 11),
        glm::uvec3(8, 7, 11),
        glm::uvec3(9, 8, 11),
        glm::uvec3(10, 9, 11)
    };

    return data;
}


