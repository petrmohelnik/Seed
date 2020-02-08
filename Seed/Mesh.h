#pragma once
#include "SubMesh.h"

class btCollisionShape;

class Mesh
{
public:
    Mesh();
    ~Mesh();
    Mesh(Mesh&&) = default;
    Mesh(const Mesh&) = delete;
    Mesh& operator=(Mesh&&) = default;
    Mesh& operator=(const Mesh&) = delete;
    std::shared_ptr<Mesh> Copy();

    int NumberOfSubmeshes();
    void DeleteDataAfterColliderLoad(bool deleteAfterLoad);

protected:
    friend class FileSystem;
    friend class MeshRenderer;
    friend class MeshCollider;
    friend class PhysicsEngine;

    void Load();
    void Unload();
    void DeleteData();

    void BindSubMesh(int index);
    
    int SubmeshIndicesCount(int index);

private:
    std::vector<std::shared_ptr<SubMesh>> subMeshes;

    bool deleteAfterColliderLoad = false;
};
