#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
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
