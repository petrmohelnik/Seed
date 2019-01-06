#include "Component.h"

Component::Component()
{
}

void Component::SetObject(std::weak_ptr<Object> parentObject)
{
	object = parentObject;
}

std::shared_ptr<Object> Component::GetObject()
{
	if (auto objectOwnership = object.lock())
		return objectOwnership;
		
	assert(false && "Component should not exist when its object does not exist");
	throw std::exception("Component should not exist when its object does not exist");
}
