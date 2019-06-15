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
    return glm::translate(position) * glm::toMat4(orientation) * scale;
}

glm::mat4 Transform::CalculateWorldMatrix()
{
    if (!isRoot)
        return parent->CalculateWorldMatrix() * CalculateLocalMatrix();

    return modelMatrix;
}

void Transform::DecomposeMatrix(glm::mat4 matrix, glm::vec3& decomposedTranslation, glm::quat& decomposedOrientation, glm::mat4& decomposedScale)
{
	glm::vec3 scale, skew;
	glm::vec4 dummyPerspective;
	glm::decompose(matrix, scale, decomposedOrientation, decomposedTranslation, skew, dummyPerspective);
    
    auto shearX = glm::mat4(1.0f);
    auto shearY = glm::mat4(1.0f);
    auto shearZ = glm::mat4(1.0f);
    shearX[2][1] = skew.x;
    shearY[2][0] = skew.y;
    shearZ[1][0] = skew.z;
    decomposedScale = shearX * shearY * shearZ * glm::scale(scale);
}

void Transform::TransformInWorldAndDecompose(glm::mat4 worldTransformation, glm::vec3& decomposedTranslation, glm::quat& decomposedOrientation, glm::mat4& decomposedScale)
{
	auto localToWorldMatrix = GetLocalToWorldMatrix();
	auto worldTransformationLocalMatrix = glm::inverse(localToWorldMatrix) * worldTransformation * localToWorldMatrix * CalculateLocalMatrix();

	DecomposeMatrix(worldTransformationLocalMatrix, decomposedTranslation, decomposedOrientation, decomposedScale);
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
    Rotate(glm::angleAxis(angle, axis), space);
}

void Transform::Rotate(glm::vec3 angles, Space space)
{
    Rotate(glm::quat(angles), space);
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

void Transform::Rotate(glm::quat quaternion, Space space)
{
    if (space == Space::Local)
        orientation = glm::normalize(orientation * quaternion);
    else
    {
        glm::vec3 dummyTranslation;
        TransformInWorldAndDecompose(glm::toMat4(quaternion), dummyTranslation, orientation, scale);
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
        
    return parent->GetRotation() * GetLocalRotation();
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
        glm::mat4 dummyScale;
        TransformInWorldAndDecompose(glm::translate(translation), position, dummyRotation, dummyScale);
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
        position = GetWorldToLocalMatrix() * glm::vec4(position_, 1.0f);
    }
}

glm::vec3 Transform::GetPosition()
{
    return GetLocalToWorldMatrix() * glm::vec4(position, 1.0f);
}

glm::vec3 Transform::GetLocalPosition()
{
    return position;
}

glm::vec3 Transform::GetRightAxis()
{
    return glm::normalize(glm::vec3(CalculateWorldMatrix()[0]));
}

glm::vec3 Transform::GetUpAxis()
{
    return glm::normalize(glm::vec3(CalculateWorldMatrix()[1]));
}

glm::vec3 Transform::GetForwardAxis()
{
    return glm::normalize(glm::vec3(CalculateWorldMatrix()[2]));
}

void Transform::SetScale(glm::vec3 scale_)
{
    scale[0][0] = scale_.x;
    scale[1][1] = scale_.y;
    scale[2][2] = scale_.z;
}

glm::vec3 Transform::GetScale()
{
    if (IsParentRoot())
        return GetLocalScale();
        
    return parent->GetScale() * GetLocalScale();
}

glm::vec3 Transform::GetLocalScale()
{
    return glm::vec3(scale[0][0], scale[1][1], scale[2][2]);
}

void Transform::LookAt(glm::vec3 position, glm::vec3 upAxis)
{
    auto const localToWorldMatrix = GetLocalToWorldMatrix();
    auto const currentWorldPosition = glm::vec3(localToWorldMatrix * glm::vec4(position, 1.0f));
    glm::vec3 dummyTranslation;
    DecomposeMatrix(glm::inverse(localToWorldMatrix) * glm::inverse(glm::lookAt(currentWorldPosition, position, upAxis)), dummyTranslation, orientation, scale);
}

void Transform::LookAtLocal(glm::vec3 position, glm::vec3 upAxis)
{
    glm::vec3 dummyTranslation;
    DecomposeMatrix(glm::lookAt(glm::vec3(0.0f), position, upAxis), dummyTranslation, orientation, scale);
}

void Transform::RotateAround(float angle, glm::vec3 worldAxis, glm::vec3 worldPoint)
{
    auto const worldTransformationMatrix = glm::translate(worldPoint) * glm::toMat4(glm::angleAxis(angle, worldAxis)) * glm::translate(-worldPoint);
    TransformInWorldAndDecompose(worldTransformationMatrix, position, orientation, scale);
}
