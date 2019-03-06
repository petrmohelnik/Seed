#include "Transform.h"
#include "Object.h"

Transform::Transform(Object* object, Transform* root)
    : Component(object)
{
    parent = root;
    parent->AddChild(this);
}

Transform* Transform::GetParent()
{
    if (IsParentRoot())
        return nullptr;

    return parent;
}

bool Transform::IsParentRoot() const
{
    return parent->isRoot;
}

void Transform::MakeRoot()
{
    isRoot = true;
}

void Transform::SetParent(Transform* parent_)
{
    std::experimental::erase_if(parent->children, [this](const Transform* parentChild)
    {
        return parentChild == this;
    });

    parent = parent_;
    parent->AddChild(this);
}

void Transform::SetParent(Object* parent_)
{
    SetParent(parent_->GetComponent<Transform>());
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

void Transform::UpdateModelMatrix()
{
    if (!isRoot)
    {
        modelMatrix = parent->modelMatrix;
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = modelMatrix * glm::toMat4(rotation);
        modelMatrix = glm::scale(modelMatrix, scale);
    }

    for (auto const& child : children)
    {
        child->UpdateModelMatrix();
    }
}

void Transform::RotateAngle(float angle, glm::vec3 axis)
{
    rotation = glm::rotate(rotation, angle, axis);
}

void Transform::RotateX(float angle)
{
    rotation = rotation * glm::angleAxis(angle, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Transform::RotateY(float angle)
{
    rotation = glm::rotate(rotation, angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Transform::RotateZ(float angle)
{
    rotation = glm::rotate(rotation, angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Transform::RotateQuat(glm::quat quaternion)
{
    rotation = rotation * quaternion;
}

void Transform::LookAt(glm::vec3 position)
{
}

glm::vec3 Transform::GetEulerAngles()
{
    if (IsParentRoot())
        return GetLocalEulerAngles();
        
    return parent->GetEulerAngles() * GetLocalEulerAngles();
}

glm::vec3 Transform::GetLocalEulerAngles()
{
    return glm::eulerAngles(rotation);
}

glm::quat Transform::GetRotation()
{
    if (IsParentRoot())
        return GetLocalRotation();
        
    return parent->GetRotation() * GetLocalRotation();
}

glm::quat Transform::GetLocalRotation()
{
    return rotation;
}

void Transform::Translate(glm::vec3 translation)
{
    position += translation;
}

void Transform::TranslateX(float translation)
{
    position.x += translation;
}

void Transform::TranslateY(float translation)
{
    position.y += translation;
}

void Transform::TranslateZ(float translation)
{
    position.z += translation;
}

glm::vec3 Transform::GetPosition()
{
    if (IsParentRoot())
        return GetLocalPosition();
        
    return parent->GetPosition() * GetLocalPosition();
}

glm::vec3 Transform::GetLocalPosition()
{
    return position;
}

glm::vec3 Transform::GetForwardAxis()
{
    return glm::vec3();
}

void Transform::SetScale(glm::vec3 scale_)
{
    scale = scale_;
}

glm::vec3 Transform::GetScale()
{
    if (IsParentRoot())
        return GetLocalScale();
        
    return parent->GetScale() * GetLocalScale();
}

glm::vec3 Transform::GetLocalScale()
{
    return scale;
}
