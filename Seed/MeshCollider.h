#pragma once
#include "Collider.h"
#include "Mesh.h"

class MeshCollider : public Collider
{
public:
    MeshCollider(Object* object, std::shared_ptr<Mesh> mesh, bool convex);
    Type GetType() override;

    int NumberOfSubmeshes();
    std::vector<glm::vec3> const& GetVertices(int submeshIndex);
    std::vector<glm::uvec3> const& GetIndices(int submeshIndex);
    bool IsConvex() const;

protected:
    friend class PhysicsEngine;

    void AssignBtCollisionShape(btCollisionShape* collisionShape);

private:
    MeshCollider* Clone() override;
    glm::vec3 GetPosition() const override;
    void SetPosition(glm::vec3 position) override;

    std::shared_ptr<Mesh> mesh;
    bool convex;

    std::shared_ptr<btCollisionShape> btShape;
};

