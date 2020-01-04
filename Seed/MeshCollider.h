#pragma once
#include "Collider.h"
#include "SubMesh.h"

class MeshCollider : public Collider
{
public:
    using Collider::Collider;

    void SetMesh(std::shared_ptr<SubMesh> mesh);
    SubMesh* GetMesh();

private:
    std::shared_ptr<SubMesh> mesh;
};

