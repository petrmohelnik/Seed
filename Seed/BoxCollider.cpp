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
    GetTransform()->Translate(center);
    auto position = GetTransform()->GetPosition();
    GetTransform()->Translate(-center);

    return position;
}

void BoxCollider::SetPosition(glm::vec3 position)
{
    auto worldPosition = GetTransform()->GetPosition();
    GetTransform()->Translate(center);
    auto worldPositionWithOffset = GetTransform()->GetPosition();
    GetTransform()->Translate(-center);

    GetTransform()->SetPosition(position - (worldPositionWithOffset - worldPosition), Transform::Space::World);
}
