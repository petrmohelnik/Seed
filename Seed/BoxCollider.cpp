#include "stdafx.h"
#include "BoxCollider.h"
#include "Transform.h"

void BoxCollider::SetBox(glm::vec3 center_, glm::vec3 size_)
{
    center = center_;
    size = size_;
}

glm::vec3 BoxCollider::GetCenter()
{
    return center;
}

glm::vec3 BoxCollider::GetSize()
{
    return size;
}

glm::vec3 BoxCollider::GetPosition() const
{
	return GetTransform()->GetLocalToWorldMatrix() * glm::vec4(GetTransform()->GetLocalPosition() + center, 1.0f);
}

void BoxCollider::SetPosition(glm::vec3 position)
{
	auto worldPosition = GetTransform()->GetPosition();
	auto worldPositionWithOffset = GetPosition();

	GetTransform()->SetPosition(position - (worldPositionWithOffset - worldPosition), Transform::Space::World);
}
