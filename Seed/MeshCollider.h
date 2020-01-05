#pragma once
#include "Collider.h"
#include "Mesh.h"

class MeshCollider : public Collider
{
public:
    using Collider::Collider;

    void SetMesh(std::shared_ptr<Mesh> mesh, int submeshIndex = 0);
    SubMesh* GetMesh();

private:
    std::shared_ptr<SubMesh> mesh;
};

