#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

std::shared_ptr<Mesh> Mesh::Copy()
{
    auto clonedMesh = std::make_shared<Mesh>();

    clonedMesh->subMeshes.reserve(subMeshes.size());
    for (auto& subMesh : subMeshes)
        clonedMesh->subMeshes.push_back(subMesh->Copy());
    
    return clonedMesh;
}

void Mesh::Load()
{
    for (auto& subMesh : subMeshes)
    {
        if (Tessellation)
            subMesh->LoadTessellation();
        else
            subMesh->Load();
    }
}

void Mesh::Unload()
{
    for (auto& subMesh : subMeshes)
    {
        subMesh->Unload();
    }
}

void Mesh::DeleteData()
{
    for (auto& subMesh : subMeshes)
    {
        subMesh->DeleteData();
    }
}

void Mesh::BindSubMesh(int index)
{
    subMeshes[index]->Bind();
}

int Mesh::NumberOfSubmeshes()
{
    return static_cast<int>(subMeshes.size());
}

void Mesh::DeleteDataAfterColliderLoad(bool deleteAfterLoad)
{
    deleteAfterColliderLoad = deleteAfterLoad;
}

int Mesh::SubmeshIndicesCount(int index)
{
    return subMeshes[index]->GetIndicesCount();
}
