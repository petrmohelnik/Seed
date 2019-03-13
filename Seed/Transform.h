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
    void SetParent(Transform* parent_);
    void SetParent(Object* parent_);

    void RotateAngle(float angle, glm::vec3 axis, Space space = Space::Local);
    void Rotate(glm::vec3 angles);
    void RotateX(float angle, Space space = Space::Local);
    void RotateY(float angle, Space space = Space::Local);
    void RotateZ(float angle, Space space = Space::Local);
    void RotateQuat(glm::quat quaternion);
    void LookAt(glm::vec3 position);
    glm::vec3 GetEulerAngles();
    glm::vec3 GetLocalEulerAngles();
    glm::quat GetRotation();
    glm::quat GetLocalRotation();
    void SetLocalRotation(glm::quat rotation);
    void Translate(glm::vec3 translation);
    void TranslateX(float translation);
    void TranslateY(float translation);
    void TranslateZ(float translation);
    void SetPosition(glm::vec3 position_, Space space = Space::Local);
    glm::vec3 GetPosition();
    glm::vec3 GetLocalPosition();
    glm::vec3 GetRightAxis();
    glm::vec3 GetUpAxis();
    glm::vec3 GetForwardAxis();
    void SetScale(glm::vec3 scale_);
    glm::vec3 GetScale();
    glm::vec3 GetLocalScale();

    glm::mat4 GetModelMatrix();

protected:
    friend class Object;
    friend class Objects;
    friend class Components;
    void DestroyAllChildren();
    void CleanChildren();
    void UpdateModelMatrix();

private:
    void AddChild(Transform* child);
    bool IsParentRoot() const;
    void MakeRoot();
    glm::mat4 CalculateLocalMatrix();
    glm::mat4 CalculateWorldMatrix();

    bool isRoot = false;

    Transform* parent = nullptr;
    std::vector<Transform*> children;

    glm::vec3 position = glm::vec3(0.0);
    glm::quat orientation = glm::angleAxis(0.0f, glm::vec3(0.0, 0.0, 1.0));
    glm::vec3 scale = glm::vec3(1.0);

    glm::mat4 modelMatrix = glm::mat4(1.0);
};
