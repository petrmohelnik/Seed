#include "Transform.h"
#include "Object.h"

Transform * Transform::GetParent()
{
    return parent;
}

void Transform::SetParent(Transform* parent_)
{
    parent = parent_;
    parent->AddChild(this);
}

void Transform::SetParent(Object* parent_)
{
    parent = parent_->GetComponent<Transform>();
    parent->AddChild(this);
}

void Transform::DestroyAllChildren()
{
    for (const auto& child : children)
    {
        child->GetObject()->Destroy();
        child->GetObject()->UpdateForDestruction();
    }
}

void Transform::CleanChildren()
{
    if (children.empty())
        return;
    std::experimental::erase_if(children, [](const auto& child)
    {
        return child->ToBeDestroyed();
    });
}

void Transform::AddChild(Transform* child)
{
    children.push_back(child);
}

glm::mat4 Transform::GetModelMatrix()
{
    return modelMatrix;
}
