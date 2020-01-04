#pragma once
#include "Component.h"

class Rigidbody;

class Collider : public Component
{
public:
    using Component::Component;
    virtual ~Collider() = default;

private:
    Rigidbody* GetRigidbody();
};
