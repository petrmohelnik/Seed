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
    return mesh->NumberOfSubmeshes() == 0 ? Type::Mesh : Type::CompoundMesh;
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

glm::vec3 MeshCollider::GetPosition() const
{
    return GetTransform()->GetPosition();
}

void MeshCollider::SetPosition(glm::vec3 position)
{
    GetTransform()->SetPosition(position, Transform::Space::World);
}

void DeleteBtSubShape(btCollisionShape* collisionSubShape)
{
    auto shapeType = collisionSubShape->getShapeType();
    if (shapeType == TRIANGLE_MESH_SHAPE_PROXYTYPE || shapeType == GIMPACT_SHAPE_PROXYTYPE)
    {
        btTriangleIndexVertexArray* triangleMeshData = nullptr;
        if (collisionSubShape->getShapeType() == TRIANGLE_MESH_SHAPE_PROXYTYPE)
            triangleMeshData = static_cast<btTriangleIndexVertexArray*>(static_cast<btTriangleMeshShape*>(collisionSubShape)->getMeshInterface());
        if (collisionSubShape->getShapeType() == GIMPACT_SHAPE_PROXYTYPE)
            triangleMeshData = static_cast<btTriangleIndexVertexArray*>(static_cast<btGImpactMeshShape*>(collisionSubShape)->getMeshInterface());

        auto indexedVerticesArray = triangleMeshData->getIndexedMeshArray();

        delete triangleMeshData;
    }
    delete collisionSubShape;
}

void MeshCollider::AssignBtCollisionShape(btCollisionShape* collisionShape)
{
    btShape = std::shared_ptr<btCollisionShape>(collisionShape, [collisionShape](btCollisionShape* btShape)
    {
        if (btShape->getShapeType() == COMPOUND_SHAPE_PROXYTYPE)
        {
            auto compoundShape = static_cast<btCompoundShape*>(btShape);
            for (int i = 0; i < compoundShape->getNumChildShapes(); i++)
                DeleteBtSubShape(compoundShape->getChildShape(i));
        }
        else
            DeleteBtSubShape(collisionShape);
    });
    mesh->btCollisionShape = btShape;

    if (mesh->deleteAfterColliderLoad)
        mesh->DeleteData();
}
