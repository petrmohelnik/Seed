#include "Component.h"
#include "Object.h"
#include "Time.h"

Component::Component(Object* object) : object(object)
{
}

Object* Component::GetObject()
{
    return object;
}

void Component::Destroy(Uint32 delay)
{
    if (timeToDestruction >= static_cast<Sint32>(delay))
        timeToDestruction = static_cast<Sint32>(delay);
    registeredForDestruction = true;
}

bool Component::UpdateForDestruction()
{
    if (!registeredForDestruction)
        return false;

    timeToDestruction -= Time::DeltaTime();

    return timeToDestruction <= 0;
}

bool Component::ToBeDestroyed()
{
    return registeredForDestruction && timeToDestruction <= 0;
}
