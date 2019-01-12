#pragma once
#include "Component.h"

class Collider : public Component
{
public:
    using Component::Component;
    virtual ~Collider() = default;
};
