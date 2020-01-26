#pragma once
#include "SubMesh.h"

class Mesh
{
public:
    Mesh();
    ~Mesh();
    Mesh(Mesh&&) = default;
    Mesh(const Mesh&) = delete;
    Mesh& operator=(Mesh&&) = default;
    Mesh& operator=(const Mesh&) = delete;
    std::shared_ptr<Mesh> Clone();

    void Load();
    void Unload();

    void BindSubMesh(int index);

    int NumberOfSubmeshes();
    int SubmeshIndicesCount(int index);

protected:
    friend class FileSystem;
    friend class MeshRenderer;
    friend class MeshCollider;
    
private:
    std::vector<std::shared_ptr<SubMesh>> subMeshes;
};
