#include "stdafx.h"
#include "BoxCollider.h"
#include "Transform.h"

BoxCollider::BoxCollider(Object* object, glm::vec3 center, glm::vec3 size) 
    : Collider(object)
    , center(center)
    , size(size)
{
}

Collider::Type BoxCollider::GetType()
{
    return Type::Box;
}

glm::vec3 BoxCollider::GetCenter()
{
    return center;
}

glm::vec3 BoxCollider::GetSize()
{
    return size;
}

BoxCollider* BoxCollider::Clone()
{
    return new BoxCollider(*this);
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
