#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
public:
    SphereCollider(Object* object, glm::vec3 center, float radius);
    Type GetType() override;

    glm::vec3 GetCenter();
    float GetRadius();

private:
    SphereCollider* Clone() override;
    glm::vec3 GetPosition() const override;
    void SetPosition(glm::vec3 position) override;

    glm::vec3 center;
    float radius;
};

