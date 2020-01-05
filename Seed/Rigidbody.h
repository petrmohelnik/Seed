#pragma once
#include "Component.h"

class Rigidbody final : public Component
{
public:
    using Component::Component;

    void SetIsKinematic(bool isKinematic);
    bool GetIsKinematic();
    void SetMass(float mass);
    float GetMass();

private:
    bool isKinematic = false;
    float mass = 0.0;
};
