#pragma once
#include "SubMesh.h"

class Mesh
{
public:
    Mesh();
    ~Mesh();
    Mesh(Mesh&& m) = default;
    Mesh(const Mesh& m) = default;
    Mesh& operator=(const Mesh& m) = default;
    Mesh& operator=(Mesh&& m) = default;

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
    void LoadSubMesh(SubMesh* subMesh);

    std::vector<std::shared_ptr<SubMesh>> subMeshes;
};
