#pragma once
#include "Collider.h"

class CapsuleCollider : public Collider
{
public:
    using Collider::Collider;

    void SetCapsule(glm::vec3 center, float height, float radius);
    glm::vec3 GetCenter();
    float GetHeight();
    float GetRadius();

private:
    glm::vec3 GetPosition() const override;

    glm::vec3 center;
    float height;
    float radius;
};

