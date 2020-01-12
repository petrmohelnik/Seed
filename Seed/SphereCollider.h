#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
public:
    using Collider::Collider;

    void SetSphere(glm::vec3 center, float radius);
    glm::vec3 GetCenter();
    float GetRadius();

private:
    glm::vec3 GetPosition() const override;
    void SetPosition(glm::vec3 position) override;

    glm::vec3 center;
    float radius;
};

