#pragma once
#include "Component.h"

class Transform : public Component
{
public:

private:
    glm::mat3 position;
    glm::mat3 rotation;
};
