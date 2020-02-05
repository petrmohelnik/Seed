#pragma once
#include "Collider.h"

class CapsuleCollider : public Collider
{
public:
    CapsuleCollider(Object* object, glm::vec3 center, float height, float radius);
    Type GetType() override;

    glm::vec3 GetCenter();
    float GetHeight();
    float GetRadius();

private:
    CapsuleCollider* Clone() override;
    glm::vec3 GetPosition() const override;
    void SetPosition(glm::vec3 position) override;

    glm::vec3 center;
    float height;
    float radius;
};

