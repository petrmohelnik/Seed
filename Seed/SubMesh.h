#pragma once
#include "AABB.h"

class SubMesh
{
public:
    SubMesh();
    ~SubMesh();
    SubMesh(SubMesh&&) = default;
    SubMesh(const SubMesh&) = delete;
    SubMesh& operator=(SubMesh&&) = default;
    SubMesh& operator=(const SubMesh&) = delete;
    std::shared_ptr<SubMesh> Copy();

    void Load();
    void Unload();

    void Bind();

    int GetIndicesCount();
    const AABB& GetAABB();

protected:
    friend class FileSystem;
    friend class Mesh;
    friend class MeshCollider;
    friend class PhysicsEngine;

    void DeleteData();

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::uvec3> indices;
    int indicesCount;
    GLuint vao = 0;
    GLuint vbo[6];

    AABB aabb;

    std::weak_ptr<btCollisionShape> btCollisionShape;

    bool deleteAfterLoad = true;
};

