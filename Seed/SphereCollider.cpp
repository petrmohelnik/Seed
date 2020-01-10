#include "stdafx.h"
#include "SphereCollider.h"
#include "Transform.h"

void SphereCollider::SetSphere(glm::vec3 center_, float radius_)
{
    center = center_;
    radius = radius_;
}

glm::vec3 SphereCollider::GetCenter()
{
    return center;
}

float SphereCollider::GetRadius()
{
    return radius;
}

glm::vec3 SphereCollider::GetPosition() const
{
    GetTransform()->Translate(center);
    auto position = GetTransform()->GetPosition();
    GetTransform()->Translate(-center);

    return position;
}
