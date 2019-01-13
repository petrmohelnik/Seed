#include "Objects.h"
#include "Object.h"
#include "Components.h"

Objects::UniqueId Objects::uniqueId = 0;

Objects::Objects(Components& components) : components(components)
{
}

Object* Objects::GetObjectByName(const std::string& name)
{
    for (const auto& object : objects)
    {
        if (object.second->GetName() == name)
            return object.second.get();
    }
    return nullptr;
}

std::vector<Object*> Objects::GetObjectsByName(const std::string& name)
{
    std::vector<Object*> result;
    for (const auto& object : objects)
    {
        if (object.second->GetName() == name)
            result.push_back(object.second.get());
    }
    return result;
}

std::vector<Object*> Objects::GetObjectsByTag(const std::string& tag)
{
    std::vector<Object*> result;
    for (const auto& object : objects)
    {
        if (object.second->ContainsTag(tag))
            result.push_back(object.second.get());
    }
    return result;
}

void Objects::RegisterForDestruction(UniqueId uniqueId)
{
    objectsToBeDestroyed.insert(uniqueId);
}

void Objects::RegisterForComponentDestruction(UniqueId uniqueId)
{
    objectsToDestroyComponents.insert(uniqueId);
}

Objects::UniqueId Objects::CreateUniqueId()
{
    return uniqueId++;
}

void Objects::Destroy()
{
    for (const auto& id : objectsToDestroyComponents)
    {
        objects[id]->DestroyComponents();
    }
    objectsToDestroyComponents.clear();

    for (const auto& id : objectsToBeDestroyed)
    {
        objects.erase(id);
    }
    objectsToBeDestroyed.clear();
}
