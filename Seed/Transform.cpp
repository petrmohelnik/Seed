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

void Transform::DecomposeMatrix(glm::mat4 matrix, glm::vec3& decomposedTranslation, glm::quat& decomposedOrientation)
{
	glm::vec3 dummyScale, dummySkew;
	glm::vec4 dummyPerspective;
	glm::decompose(matrix, dummyScale, decomposedOrientation, decomposedTranslation, dummySkew, dummyPerspective);
}

void Transform::TransformInWorldAndDecompose(glm::mat4 worldTransformation, glm::vec3& decomposedTranslation, glm::quat& decomposedOrientation)
{
	auto localToWorldMatrix = GetLocalToWorldMatrix();
	auto worldTransformationLocalMatrix = glm::inverse(localToWorldMatrix) * worldTransformation * localToWorldMatrix * CalculateLocalMatrix();

	DecomposeMatrix(worldTransformationLocalMatrix, decomposedTranslation, decomposedOrientation);
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

glm::mat4 Transform::GetLocalToWorldMatrix()
{
	return parent->CalculateWorldMatrix();
}

glm::mat4 Transform::GetWorldToLocalMatrix()
{
	return glm::inverse(GetLocalToWorldMatrix());
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
    RotateQuat(glm::angleAxis(angle, axis), space);
}

void Transform::Rotate(glm::vec3 angles, Space space)
{
    RotateQuat(glm::quat(angles), space);
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

void Transform::RotateQuat(glm::quat quaternion, Space space)
{
    if (space == Space::Local)
        orientation = glm::normalize(orientation * quaternion);
    else
    {
        glm::vec3 dummyTranslation;
        TransformInWorldAndDecompose(glm::toMat4(quaternion), dummyTranslation, orientation);
    }
}

void Transform::SetRotation(glm::quat rotation)
{
    orientation = rotation;
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

void Transform::Translate(glm::vec3 translation, Space space)
{
    if(space == Space::Local)
        position += translation;
    else
    {
        glm::quat dummyRotation;
        TransformInWorldAndDecompose(glm::translate(translation), position, dummyRotation);
    }
}

void Transform::TranslateX(float translation, Space space)
{
    Translate(glm::vec3(translation, 0.0f, 0.0f), space);
}

void Transform::TranslateY(float translation, Space space)
{
    Translate(glm::vec3(0.0f, translation, 0.0f), space);
}

void Transform::TranslateZ(float translation, Space space)
{
    Translate(glm::vec3(0.0f, 0.0f, translation), space);
}

void Transform::SetPosition(glm::vec3 position_, Space space)
{
    if (space == Space::Local)
    {
        position = position_;
    }
    else
    {
        position = glm::vec3(GetWorldToLocalMatrix() * glm::vec4(position_, 1.0f));
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
    return glm::vec3(CalculateWorldMatrix()[0]);
}

glm::vec3 Transform::GetUpAxis()
{
    return glm::vec3(CalculateWorldMatrix()[1]);
}

glm::vec3 Transform::GetForwardAxis()
{
    return glm::vec3(CalculateWorldMatrix()[2]);
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
