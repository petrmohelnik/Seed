#pragma once
#include "Component.h"

class Transform final : public Component
{
public:
    using Component::Component;

private:
    glm::mat3 position;
    glm::mat3 rotation;
};
