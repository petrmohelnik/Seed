#include "Collider.h"
#include "Object.h"
#include "Transform.h"

void Collider::SetIsKinematic(bool isKinematic_)
{
    isKinematic = isKinematic_;
}

bool Collider::IsKinematic() const
{
    return isKinematic;
}

void Collider::SetIsTrigger(bool isTrigger_)
{
    isTrigger = isTrigger_;
}

bool Collider::IsTrigger() const
{
    return isTrigger;
}

void Collider::SetMass(float mass_)
{
    mass = mass_;
}

float Collider::GetMass() const
{
    return mass;
}
