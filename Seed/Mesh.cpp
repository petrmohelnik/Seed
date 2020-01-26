#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

std::shared_ptr<Mesh> Mesh::Clone()
{
    auto clonedMesh = std::make_shared<Mesh>();

    clonedMesh->subMeshes.reserve(subMeshes.size());
    for (auto& subMesh : subMeshes)
        clonedMesh->subMeshes.push_back(subMesh->Clone());
    
    return clonedMesh;
}

void Mesh::Load()
{
    for (auto& subMesh : subMeshes)
    {
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

void Mesh::BindSubMesh(int index)
{
    subMeshes[index]->Bind();
}

int Mesh::NumberOfSubmeshes()
{
    return static_cast<int>(subMeshes.size());
}

int Mesh::SubmeshIndicesCount(int index)
{
    return subMeshes[index]->GetIndicesCount();
}
