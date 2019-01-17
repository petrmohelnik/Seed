#pragma once
#include "Component.h"

class Camera final : public Component
{
public:
    using Component::Component;

private:
    GLuint projection;
};
