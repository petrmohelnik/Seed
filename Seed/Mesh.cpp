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
}

void Mesh::LoadSubMesh(SubMesh& subMesh)
{
    GLuint vbo[4];
    glGenBuffers(4, vbo);
    glGenVertexArrays(1, &subMesh.vao);
    glBindVertexArray(subMesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, subMesh.vertices.size() * sizeof(float) * 3, &subMesh.vertices[0].x, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, subMesh.normals.size() * sizeof(float) * 3, &subMesh.normals[0].x, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, subMesh.texCoords.size() * sizeof(float) * 2, &subMesh.texCoords[0].x, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, subMesh.indices.size() * sizeof(Uint32) * 3, &subMesh.indices[0].x, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Mesh::Unload()
{
    for (auto& subMesh : subMeshes)
    {
        glDeleteBuffers(1, &subMesh.vao);
    }
}

int Mesh::NumberOfSubmeshes()
{
    return static_cast<int>(subMeshes.size());
}
