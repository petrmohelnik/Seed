#pragma once
#include "Collider.h"
#include "Mesh.h"

class MeshCollider : public Collider
{
public:
    MeshCollider(Object* object, std::shared_ptr<Mesh> mesh, bool convex);

    int NumberOfSubmeshes();
    std::vector<glm::vec3> const& GetVertices(int submeshIndex);
    std::vector<glm::uvec3> const& GetIndices(int submeshIndex);
    bool IsConvex() const;

private:
    glm::vec3 GetPosition() const override;
    void SetPosition(glm::vec3 position) override;

    std::shared_ptr<Mesh> mesh;
    bool convex;
};

