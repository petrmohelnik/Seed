#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
public:
    BoxCollider(Object* object, glm::vec3 center, glm::vec3 size);
    Type GetType() override;

    glm::vec3 GetCenter();
    glm::vec3 GetSize();

private:
    BoxCollider* Clone() override;
    glm::vec3 GetPosition() const override;
    void SetPosition(glm::vec3 position) override;

    glm::vec3 center;
    glm::vec3 size;
};

