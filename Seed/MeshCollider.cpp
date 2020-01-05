#include "stdafx.h"
#include "MeshCollider.h"

void MeshCollider::SetMesh(std::shared_ptr<Mesh> mesh_, int submeshIndex)
{
    mesh = mesh_->subMeshes[submeshIndex];
}

std::vector<glm::vec3> const & MeshCollider::GetVertices()
{
    return mesh->vertices;
}

std::vector<glm::uvec3> const & MeshCollider::GetIndices()
{
    return mesh->indices;
}
