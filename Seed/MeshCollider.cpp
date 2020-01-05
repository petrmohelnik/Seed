#include "stdafx.h"
#include "MeshCollider.h"

void MeshCollider::SetMesh(std::shared_ptr<Mesh> mesh_, int submeshIndex)
{
    mesh = mesh_->subMeshes[submeshIndex];
}

SubMesh* MeshCollider::GetMesh()
{
    return mesh.get();
}
