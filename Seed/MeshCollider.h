#pragma once
#include "Collider.h"
#include "Mesh.h"

class MeshCollider : public Collider
{
public:
    using Collider::Collider;

    void SetMesh(std::shared_ptr<Mesh> mesh, int submeshIndex = 0);
    std::vector<glm::vec3> const& GetVertices();
    std::vector<glm::uvec3> const& GetIndices();
    void SetConcave();
    bool IsConvex() const;

private:
    glm::vec3 GetPosition() const override;
    void SetPosition(glm::vec3 position) override;

    std::shared_ptr<SubMesh> mesh;
    bool convex = true;
};

