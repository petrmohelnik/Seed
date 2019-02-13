#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
    Unload();
}

void Mesh::Load()
{
    for (auto& subMesh : subMeshes)
    {
        LoadSubMesh(subMesh);
    }

    if (deleteAfterLoad)
    {
        for (auto& subMesh : subMeshes)
        {
            std::vector<glm::vec3> emptyVertices;
            subMesh.vertices.swap(emptyVertices);
            std::vector<glm::vec3> emptyNormals;
            subMesh.normals.swap(emptyNormals);
            std::vector<glm::vec2> emptyTexCoords;
            subMesh.texCoords.swap(emptyTexCoords);
            std::vector<glm::uvec3> emptyIndices;
            subMesh.indices.swap(emptyIndices);
        }
    }
}

void Mesh::LoadSubMesh(SubMesh& subMesh)
{
    glGenBuffers(4, &subMesh.vbo[0]);
    glGenVertexArrays(1, &subMesh.vao);
    glBindVertexArray(subMesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, subMesh.vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, subMesh.vertices.size() * sizeof(float) * 3, &subMesh.vertices[0].x, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, subMesh.vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, subMesh.normals.size() * sizeof(float) * 3, &subMesh.normals[0].x, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, subMesh.vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, subMesh.texCoords.size() * sizeof(float) * 2, &subMesh.texCoords[0].x, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMesh.vbo[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, subMesh.indices.size() * sizeof(Uint32) * 3, &subMesh.indices[0].x, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Mesh::Unload()
{
    for (auto& subMesh : subMeshes)
    {
        glDeleteBuffers(4, &subMesh.vbo[0]);
        glDeleteVertexArrays(1, &subMesh.vao);
    }
}

int Mesh::NumberOfSubmeshes()
{
    return static_cast<int>(subMeshes.size());
}
