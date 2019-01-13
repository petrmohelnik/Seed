#include "Component.h"
#include "Object.h"

Component::Component(Object* object) : object(object)
{
}

Object* Component::GetObject()
{
    return object;
}

void Component::Destroy()
{
    toBeDestroyed = true;
    GetObject()->RegisterForComponentDestruction();
}

bool Component::IsRegisteredForDestruction()
{
    return toBeDestroyed;
}

void Component::RegisterForDestruction()
{
    toBeDestroyed = true;
}
