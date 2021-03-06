#pragma once
#include "Component.h"

class Transform final : public Component
{
public:
    enum class Space
    {
        Local,
        World
    };

    using Component::Component;
    Transform(Object* object, Transform* root);
    Transform* GetParent();
    void Clone(Transform* transform);
    void Clone(Object* object);
    void SetParent(Transform* parent_);
    void SetParent(Object* parent_);
    int GetChildCount();
    Transform* GetChild(int index);

    void RotateAngle(float angle, glm::vec3 axis, Space space = Space::Local);
    void Rotate(glm::vec3 angles, Space space = Space::Local);
    void RotateX(float angle, Space space = Space::Local);
    void RotateY(float angle, Space space = Space::Local);
    void RotateZ(float angle, Space space = Space::Local);
    void Rotate(glm::quat quaternion, Space space = Space::Local);
    void SetRotation(glm::quat rotation, Space space = Space::Local);
    glm::vec3 GetEulerAngles();
    glm::vec3 GetLocalEulerAngles();
    glm::quat GetRotation();
    glm::quat GetLocalRotation();
    void Translate(glm::vec3 translation, Space space = Space::Local);
    void TranslateX(float translation, Space space = Space::Local);
    void TranslateY(float translation, Space space = Space::Local);
    void TranslateZ(float translation, Space space = Space::Local);
    void SetPosition(glm::vec3 position_, Space space = Space::Local);
    glm::vec3 GetPosition();
    glm::vec3 GetLocalPosition();
    glm::vec3 GetRightAxis();
    glm::vec3 GetUpAxis();
    glm::vec3 GetForwardAxis();
    void Scale(float scale_);
    void Scale(glm::vec3 scale_);
    void SetScale(float scale_);
    void SetScale(glm::vec3 scale_);
    glm::vec3 GetScale();
    glm::vec3 GetLocalScale();
    void LookAt(glm::vec3 position, glm::vec3 upAxis = glm::vec3(0.0f, 1.0f, 0.0f));
    void LookAtLocal(glm::vec3 position, glm::vec3 upAxis = glm::vec3(0.0f, 1.0f, 0.0f));
    void RotateAround(float angle, glm::vec3 worldAxis, glm::vec3 worldPoint = glm::vec3(0.0f, 0.0f, 0.0f));

    glm::mat4 GetModelMatrix();
    glm::mat4 GetLocalToWorldMatrix();
    glm::mat4 GetWorldToLocalMatrix();

protected:
    friend class Object;
    friend class Objects;
    friend class Components;

    void DestroyAllChildren();
    void CleanChildren();
    void DisconnectChildrenFromParent();
    void UpdateModelMatrix();

    void OnInputGraphUpdate() override;

private:
    Transform* Clone() override;
    void AddChild(Transform* child);
    bool IsParentRoot() const;
    void MakeRoot();
    glm::mat4 CalculateLocalMatrix();
    glm::mat4 CalculateWorldMatrix();
    void DecomposeMatrix(glm::mat4 matrix, glm::vec3& decomposedTranslation, glm::quat& decomposedOrientation, glm::mat4& decomposedScale);
    void TransformInWorldAndDecompose(glm::mat4 worldTransformation, glm::vec3& decomposedTranslation, glm::quat& decomposedOrientation, glm::mat4& decomposedScale);

    bool isRoot = false;

    Transform* parent = nullptr;
    std::vector<Transform*> children;

    glm::vec3 position = glm::vec3(0.0);
    glm::quat orientation = glm::angleAxis(0.0f, glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 scale = glm::mat4(1.0);

    glm::mat4 modelMatrix = glm::mat4(1.0);
};
