#include "Collider.h"
#include "Object.h"
#include "Transform.h"
#include "Rigidbody.h"

Rigidbody* Collider::GetRigidbody()
{
    auto object = GetObject();
    while (!object->GetComponent<Rigidbody>())
    {
        auto parent = object->GetComponent<Transform>()->GetParent();
        if (!parent)
            return nullptr;
            
        object = parent->GetObject();
    }

    return object->GetComponent<Rigidbody>();
}
