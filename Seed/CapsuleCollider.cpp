#include "stdafx.h"
#include "CapsuleCollider.h"

void CapsuleCollider::SetCapsule(glm::vec3 center_, float height_, float radius_)
{
    center = center_;
    height = height_;
    radius = radius_;
}

glm::vec3 CapsuleCollider::GetCenter()
{
    return center;
}

float CapsuleCollider::GetHeight()
{
    return height;
}

float CapsuleCollider::GetRadius()
{
    return radius;
}