#pragma once
#include "Component.h"

class Rigidbody final : public Component
{
public:
    using Component::Component;

    void SetIsKinematic(bool isKinematic);
    bool GetIsKinematic();

private:
    bool isKinematic = false;
};
