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

Object* Objects::CreateObjectWithMesh(const std::string& name, const std::string& meshFilePath, glm::vec3 position)
{
    auto object = CreateObject(name);
    object->AddComponent<MeshRenderer>()->Load(meshFilePath);
    object->GetComponent<Transform>()->Translate(position);
    return object;
}

void Objects::CreateObjectsFromScene(const std::string& path) const
{
    fileSystem.LoadObjects(path);
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

void Objects::SetSkybox(std::unique_ptr<TextureCubeMap> cubeMap)
{
    components.SetSkybox(std::make_unique<Skybox>(std::move(cubeMap)));
}

void Objects::RemoveSkybox()
{
    components.RemoveSkybox();
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

void Objects::DeleteAll()
{
    components.GetRoot()->GetComponent<Transform>()->DestroyAllChildren();
    UpdateForDestruction();
}
