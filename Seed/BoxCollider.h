#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
public:
    using Collider::Collider;

    void SetBox(glm::vec3 center, glm::vec3 size);
    glm::vec3 GetCenter();
    glm::vec3 GetSize();

private:
    glm::vec3 GetPosition() override;

    glm::vec3 center;
    glm::vec3 size;
};

