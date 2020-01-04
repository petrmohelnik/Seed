#include "stdafx.h"
#include "SphereCollider.h"

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
