#include "stdafx.h"
#include "CapsuleCollider.h"
#include "Transform.h"

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

glm::vec3 CapsuleCollider::GetPosition() const
{
	return GetTransform()->GetLocalToWorldMatrix() * glm::vec4(GetTransform()->GetLocalPosition() + center, 1.0f);
}

void CapsuleCollider::SetPosition(glm::vec3 position)
{
	auto worldPosition = GetTransform()->GetPosition();
	auto worldPositionWithOffset = GetPosition();

	GetTransform()->SetPosition(position - (worldPositionWithOffset - worldPosition), Transform::Space::World);
}
