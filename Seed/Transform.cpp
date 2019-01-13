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
    RegisterForDestruction();
    for (const auto& child : children)
        child->GetObject()->Destroy();
}

void Transform::CleanChildren()
{
    if (children.empty())
        return;
    std::experimental::erase_if(children, [](const auto& child)
    {
        return child->IsRegisteredForDestruction();
    });
}

void Transform::AddChild(Transform* child)
{
    children.push_back(child);
}
