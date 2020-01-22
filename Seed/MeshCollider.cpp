#include "stdafx.h"
#include "MeshCollider.h"
#include "Transform.h"

MeshCollider::MeshCollider(Object* object, std::shared_ptr<Mesh> mesh, bool convex, int submeshIndex)
    : Collider(object)
    , convex(convex)
    , mesh(mesh->subMeshes[submeshIndex])
{
}

std::vector<glm::vec3> const& MeshCollider::GetVertices()
{
    return mesh->vertices;
}

std::vector<glm::uvec3> const& MeshCollider::GetIndices()
{
    return mesh->indices;
}

bool MeshCollider::IsConvex() const
{
    return convex;
}

glm::vec3 MeshCollider::GetPosition() const
{
    return GetTransform()->GetPosition();
}

void MeshCollider::SetPosition(glm::vec3 position)
{
    GetTransform()->SetPosition(position, Transform::Space::World);
}
