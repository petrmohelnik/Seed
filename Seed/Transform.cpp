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
    return parent && parent->isRoot;
}

void Transform::MakeRoot()
{
    isRoot = true;
}

glm::mat4 Transform::CalculateLocalMatrix()
{
    return glm::translate(position) * glm::toMat4(orientation) * glm::scale(scale);
}

glm::mat4 Transform::CalculateWorldMatrix()
{
    if (!isRoot)
        return parent->CalculateWorldMatrix() * CalculateLocalMatrix();

    return modelMatrix;
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
        modelMatrix = parent->modelMatrix * CalculateLocalMatrix();
    }

    for (auto const& child : children)
    {
        child->UpdateModelMatrix();
    }
}

void Transform::RotateAngle(float angle, glm::vec3 axis, Space space)
{
    if (space == Space::Local)
        orientation = glm::normalize(glm::angleAxis(angle, axis) * orientation);
    else
        orientation = glm::normalize(orientation * glm::angleAxis(angle, axis));
}

void Transform::Rotate(glm::vec3 angles)
{
    orientation = glm::normalize(glm::quat(angles) * orientation);
}

void Transform::RotateX(float angle, Space space)
{
    RotateAngle(angle, glm::vec3(1.0f, 0.0f, 0.0f), space);
}

void Transform::RotateY(float angle, Space space)
{
    RotateAngle(angle, glm::vec3(0.0f, 1.0f, 0.0f), space);
}

void Transform::RotateZ(float angle, Space space)
{
    RotateAngle(angle, glm::vec3(0.0f, 0.0f, 1.0f), space);
}

void Transform::RotateQuat(glm::quat quaternion)
{
    orientation = glm::normalize(quaternion * orientation);
}

void Transform::LookAt(glm::vec3 position)
{
}

glm::vec3 Transform::GetEulerAngles()
{
    return glm::eulerAngles(GetRotation());
}

glm::vec3 Transform::GetLocalEulerAngles()
{
    return glm::eulerAngles(orientation);
}

glm::quat Transform::GetRotation()
{
    if (IsParentRoot())
        return GetLocalRotation();
        
    return GetLocalRotation() * parent->GetRotation();
}

glm::quat Transform::GetLocalRotation()
{
    return orientation;
}

void Transform::SetLocalRotation(glm::quat rotation)
{
    orientation = rotation;
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

void Transform::SetPosition(glm::vec3 position_, Space space)
{
    if (space == Space::Local)
    {
        position = position_;
    }
    else
    {
        position = glm::vec3((glm::inverse(parent->CalculateWorldMatrix())) * glm::vec4(position_, 1.0f));
    }
}

glm::vec3 Transform::GetPosition()
{
    if (IsParentRoot())
        return GetLocalPosition();
        
    return GetLocalPosition() + parent->GetPosition();
}

glm::vec3 Transform::GetLocalPosition()
{
    return position;
}

glm::vec3 Transform::GetRightAxis()
{
    auto rotationMatrix = glm::toMat4(GetRotation());
    return glm::vec3(rotationMatrix[0]);
}

glm::vec3 Transform::GetUpAxis()
{
    auto rotationMatrix = glm::toMat4(GetRotation());
    return glm::vec3(rotationMatrix[1]);
}

glm::vec3 Transform::GetForwardAxis()
{
    auto rotationMatrix = glm::toMat4(GetRotation());
    return glm::vec3(rotationMatrix[2]);
}

void Transform::SetScale(glm::vec3 scale_)
{
    scale = scale_;
}

glm::vec3 Transform::GetScale()
{
    if (IsParentRoot())
        return GetLocalScale();
        
    return GetLocalScale() * parent->GetScale();
}

glm::vec3 Transform::GetLocalScale()
{
    return scale;
}
