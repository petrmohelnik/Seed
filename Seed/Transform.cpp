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

void Transform::RotateAngle(float angle, glm::vec3 axis)
{
    glm::rotate(rotation, angle, axis);
}

void Transform::RotateX(float angle)
{
    glm::rotate(rotation, angle, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Transform::RotateY(float angle)
{
    glm::rotate(rotation, angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Transform::RotateZ(float angle)
{
    glm::rotate(rotation, angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Transform::RotateQuat(glm::quat quaternion)
{
    rotation *= quaternion;
}

void Transform::LookAt(glm::vec3 position)
{
}

glm::vec3 Transform::GetEulerAngles()
{
    return glm::vec3();
}

glm::vec3 Transform::GetLocalEulerAngles()
{
    return glm::eulerAngles(rotation);
}

glm::quat Transform::GetRotation()
{
    return glm::quat();
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
    return glm::vec3();
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
    return glm::vec3();
}

glm::vec3 Transform::GetLocalScale()
{
    return scale;
}
