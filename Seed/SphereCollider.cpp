#include "stdafx.h"
#include "SphereCollider.h"
#include "Transform.h"

SphereCollider::SphereCollider(Object* object, glm::vec3 center, float radius)
    : Collider(object)
    , center(center)
    , radius(radius)
{
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
    return GetTransform()->GetLocalToWorldMatrix() * glm::vec4(GetTransform()->GetLocalPosition() + center, 1.0f);
}

void SphereCollider::SetPosition(glm::vec3 position)
{
    auto worldPosition = GetTransform()->GetPosition();
    auto worldPositionWithOffset = GetPosition();

    GetTransform()->SetPosition(position - (worldPositionWithOffset - worldPosition), Transform::Space::World);
}
