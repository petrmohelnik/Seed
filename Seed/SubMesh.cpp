#include "SubMesh.h"
#include "AABB.h"

SubMesh::SubMesh()
{
}

SubMesh::~SubMesh()
{
    Unload();
}

std::shared_ptr<SubMesh> SubMesh::Clone()
{
    auto clonedSubMesh = std::make_shared<SubMesh>();

    clonedSubMesh->vertices = vertices;
    clonedSubMesh->normals = normals;
    clonedSubMesh->tangents = tangents;
    clonedSubMesh->bitangents = bitangents;
    clonedSubMesh->texCoords = texCoords;
    clonedSubMesh->indices = indices;
    clonedSubMesh->indicesCount = indicesCount;
    clonedSubMesh->indices = indices;
    clonedSubMesh->aabb = aabb;
    clonedSubMesh->deleteAfterLoad = deleteAfterLoad;

   /* GLuint vao = 0;
    GLuint vbo[6];*/
    throw std::runtime_error("Need to implement copy of vao/vbo to clonedSubMesh - cloned mesh cannot share gpu data");

    return clonedSubMesh;
}

void SubMesh::Load()
{
    if (vao != 0)
        return;

    glGenBuffers(6, &vbo[0]);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, &vertices[0].x, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float) * 3, &normals[0].x, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(float) * 3, &tangents[0].x, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(float) * 3, &bitangents[0].x, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float) * 2, &texCoords[0].x, GL_STATIC_DRAW);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[5]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Uint32) * 3, &indices[0].x, GL_STATIC_DRAW);
    indicesCount = static_cast<int>(indices.size()) * 3;

    glBindVertexArray(0);

    if (deleteAfterLoad)
    {
        vertices.clear(); vertices.shrink_to_fit();
        normals.clear(); normals.shrink_to_fit();
        tangents.clear(); tangents.shrink_to_fit();
        bitangents.clear(); bitangents.shrink_to_fit();
        texCoords.clear(); texCoords.shrink_to_fit();
        indices.clear(); indices.shrink_to_fit();
    }
}

void SubMesh::Unload()
{
    glDeleteBuffers(6, &vbo[0]);
    glDeleteVertexArrays(1, &vao);
    std::fill(vbo, vbo + 6, 0);
    vao = 0;
}

void SubMesh::Bind()
{
    glBindVertexArray(vao);
}

int SubMesh::GetIndicesCount()
{
    return indicesCount;
}

const AABB& SubMesh::GetAABB()
{
    return aabb;
}
