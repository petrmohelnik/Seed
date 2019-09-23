#include "Component.h"
#include "Engine.h"
#include "Object.h"
#include "Time.h"
#include "Transform.h"

Component::Component(Object* object) : object(object)
{
}

Object* Component::GetObject() const
{
    return object;
}

Transform* Component::GetTransform()
{
    return object->GetComponent<Transform>();
}

void Component::Destroy(float delay)
{
    if (timeToDestruction >= delay)
        timeToDestruction = delay;
    registeredForDestruction = true;
}

bool Component::UpdateForDestruction()
{
    if (!registeredForDestruction)
        return false;

    timeToDestruction -= Engine::GetTime().DeltaTime();

    return timeToDestruction <= 0;
}

bool Component::ToBeDestroyed()
{
    return registeredForDestruction && timeToDestruction <= 0;
}
