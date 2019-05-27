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
            subMesh.vertices.clear(); subMesh.vertices.shrink_to_fit();
            subMesh.normals.clear(); subMesh.normals.shrink_to_fit();
            subMesh.tangents.clear(); subMesh.tangents.shrink_to_fit();
            subMesh.bitangents.clear(); subMesh.bitangents.shrink_to_fit();
            subMesh.texCoords.clear(); subMesh.texCoords.shrink_to_fit();
            subMesh.indices.clear(); subMesh.indices.shrink_to_fit();
        }
    }
}

void Mesh::LoadSubMesh(SubMesh& subMesh)
{
    if (subMesh.vao != 0)
        return;

    glGenBuffers(6, &subMesh.vbo[0]);
    glGenVertexArrays(1, &subMesh.vao);
    glBindVertexArray(subMesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, subMesh.vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, subMesh.vertices.size() * sizeof(float) * 3, &subMesh.vertices[0].x, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, subMesh.vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, subMesh.normals.size() * sizeof(float) * 3, &subMesh.normals[0].x, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, subMesh.vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, subMesh.tangents.size() * sizeof(float) * 3, &subMesh.tangents[0].x, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, subMesh.vbo[3]);
    glBufferData(GL_ARRAY_BUFFER, subMesh.bitangents.size() * sizeof(float) * 3, &subMesh.bitangents[0].x, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, subMesh.vbo[4]);
    glBufferData(GL_ARRAY_BUFFER, subMesh.texCoords.size() * sizeof(float) * 2, &subMesh.texCoords[0].x, GL_STATIC_DRAW);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMesh.vbo[5]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, subMesh.indices.size() * sizeof(Uint32) * 3, &subMesh.indices[0].x, GL_STATIC_DRAW);
    subMesh.indicesCount = static_cast<int>(subMesh.indices.size()) * 3;

    glBindVertexArray(0);
}

void Mesh::Unload()
{
    for (auto& subMesh : subMeshes)
    {
        glDeleteBuffers(6, &subMesh.vbo[0]);
        glDeleteVertexArrays(1, &subMesh.vao);
        std::fill(subMesh.vbo, subMesh.vbo + 6, 0);
        subMesh.vao = 0;
    }
}

void Mesh::BindSubMesh(int index)
{
    glBindVertexArray(subMeshes[index].vao);
}

int Mesh::NumberOfSubmeshes()
{
    return static_cast<int>(subMeshes.size());
}

int Mesh::SubmeshIndicesCount(int index)
{
    return subMeshes[index].indicesCount;
}
