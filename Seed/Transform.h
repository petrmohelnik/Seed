#pragma once
#include "Component.h"

class Transform final : public Component
{
public:
    using Component::Component;
    Transform* GetParent();
    void SetParent(Transform* parent_);
    void SetParent(Object* parent_);

    void RotateAngle(float angle, glm::vec3 axis);
    void RotateX(float angle);
    void RotateY(float angle);
    void RotateZ(float angle);
    void RotateQuat(glm::quat quaternion);
    void LookAt(glm::vec3 position);
    glm::vec3 GetEulerAngles();
    glm::vec3 GetLocalEulerAngles();
    glm::quat GetRotation();
    glm::quat GetLocalRotation();
    void Translate(glm::vec3 translation);
    void TranslateX(float translation);
    void TranslateY(float translation);
    void TranslateZ(float translation);
    glm::vec3 GetPosition();
    glm::vec3 GetLocalPosition();
    glm::vec3 GetForwardAxis();
    void SetScale(glm::vec3 scale_);
    glm::vec3 GetScale();
    glm::vec3 GetLocalScale();

    glm::mat4 GetModelMatrix();

protected:
    friend class Object;
    friend class Objects;
    void DestroyAllChildren();
    void CleanChildren();

private:
    void AddChild(Transform* child);

    Transform* parent = nullptr;
    std::vector<Transform*> children;

    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    glm::mat4 modelMatrix;
};
