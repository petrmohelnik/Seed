#include "Rigidbody.h"

void Rigidbody::SetIsKinematic(bool isKinematic_)
{
    isKinematic = isKinematic_;
}

bool Rigidbody::GetIsKinematic()
{
    return isKinematic;
}

void Rigidbody::SetMass(float mass_)
{
    mass = mass_;
}

float Rigidbody::GetMass()
{
    return mass;
}
