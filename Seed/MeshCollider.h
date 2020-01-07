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

private:
    glm::vec3 GetPosition() override;

    std::shared_ptr<SubMesh> mesh;
};

