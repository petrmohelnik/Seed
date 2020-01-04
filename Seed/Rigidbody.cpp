#include "Rigidbody.h"

void Rigidbody::SetIsKinematic(bool isKinematic_)
{
    isKinematic = isKinematic_;
}

bool Rigidbody::GetIsKinematic()
{
    return isKinematic;
}
