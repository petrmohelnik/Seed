#include "Component.h"
#include "Object.h"

Component::Component(std::weak_ptr<Object> object) : object(object)
{
}

std::shared_ptr<Object> Component::GetObject()
{
	if (auto objectOwnership = object.lock())
		return objectOwnership;
		
	assert(false && "Component should not exist when its object does not exist");
	throw std::exception("Component should not exist when its object does not exist");
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
