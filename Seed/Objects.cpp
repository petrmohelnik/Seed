#include "Objects.h"
#include "Object.h"
#include "Components.h"
#include "Transform.h"
#include "FileSystem.h"
#include "Engine.h"

Objects::Objects() 
    : components(Engine::GetComponents())
    , fileSystem(Engine::GetFileSystem())
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

void Objects::UpdateForDestruction()
{
    bool isSomethingDestroyed = false;
    for (const auto& object : objects)
    {
        if(object.second->UpdateForDestruction())
            isSomethingDestroyed = true;
    }
    
    if (!isSomethingDestroyed)
        return;

    components.CleanComponents();

    for (auto object = std::begin(objects); object != std::end(objects); )
    {
        if (object->second->DoDestruction())
            object = objects.erase(object);
        else
            ++object;
    }
}
