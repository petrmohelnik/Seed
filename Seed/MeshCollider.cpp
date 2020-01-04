#include "stdafx.h"
#include "MeshCollider.h"

void MeshCollider::SetMesh(std::shared_ptr<SubMesh> mesh_)
{
    mesh = mesh_;
}

SubMesh* MeshCollider::GetMesh()
{
    return mesh.get();
}
