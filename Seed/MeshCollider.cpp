#include "stdafx.h"
#include "MeshCollider.h"
#include "Transform.h"

MeshCollider::MeshCollider(Object* object, std::shared_ptr<Mesh> mesh, bool convex)
    : Collider(object)
    , convex(convex)
    , mesh(mesh)
{
}

Collider::Type MeshCollider::GetType()
{
    return mesh->NumberOfSubmeshes() > 1 ? Type::CompoundMesh : Type::Mesh;
}

int MeshCollider::NumberOfSubmeshes()
{
    return mesh->NumberOfSubmeshes();
}

std::vector<glm::vec3> const& MeshCollider::GetVertices(int submeshIndex)
{
    return mesh->subMeshes[submeshIndex]->vertices;
}

std::vector<glm::uvec3> const& MeshCollider::GetIndices(int submeshIndex)
{
    return mesh->subMeshes[submeshIndex]->indices;
}

bool MeshCollider::IsConvex() const
{
    return convex;
}

MeshCollider* MeshCollider::Clone()
{
    return new MeshCollider(*this);
}

glm::vec3 MeshCollider::GetPosition() const
{
    return GetTransform()->GetPosition();
}

void MeshCollider::SetPosition(glm::vec3 position)
{
    GetTransform()->SetPosition(position, Transform::Space::World);
}

void MeshCollider::AssignBtCollisionShape(btCollisionShape* collisionShape, int subMeshIndex)
{
    btSubMeshShapes.push_back(std::shared_ptr<btCollisionShape>(collisionShape, [collisionShape](btCollisionShape* btShape)
    {
        auto shapeType = collisionShape->getShapeType();
        if (shapeType == TRIANGLE_MESH_SHAPE_PROXYTYPE || shapeType == GIMPACT_SHAPE_PROXYTYPE)
        {
            btTriangleIndexVertexArray* triangleMeshData = nullptr;
            if (collisionShape->getShapeType() == TRIANGLE_MESH_SHAPE_PROXYTYPE)
                triangleMeshData = static_cast<btTriangleIndexVertexArray*>(static_cast<btTriangleMeshShape*>(collisionShape)->getMeshInterface());
            if (collisionShape->getShapeType() == GIMPACT_SHAPE_PROXYTYPE)
                triangleMeshData = static_cast<btTriangleIndexVertexArray*>(static_cast<btGImpactMeshShape*>(collisionShape)->getMeshInterface());

            delete triangleMeshData;
        }
        delete collisionShape;
    }));
    mesh->subMeshes[subMeshIndex]->btCollisionShape = btSubMeshShapes.back();

    if (mesh->deleteAfterColliderLoad)
        mesh->subMeshes[subMeshIndex]->DeleteData();
}
