#pragma once

#include "Object.h"
class Component;
class Components;
class FileSystem;
class TextureCubeMap;

class Objects
{
public:
    Objects();

    template<typename T = Object>
    T* CreateObject(const std::string& name);
    template<typename T>
    T* CloneObject(T* object);
    Object* CreateObjectWithMesh(const std::string& name, const std::string& meshFilePath, glm::vec3 position = glm::vec3(0.0f), bool deleteAfterLoad = true);
    Object* CreateObjectsFromScene(const std::string& path, const std::string rootName) const;
    Object* GetObjectByName(const std::string& name);
    std::vector<Object*> GetObjectsByName(const std::string& name);
    std::vector<Object*> GetObjectsByName(std::function<bool(std::string const& name)> comparisonFunction);
    std::vector<Object*> GetObjectsByTag(const std::string& tag);
    template<typename T>
    std::vector<T*> GetObjects();
    template<typename T>
    std::vector<T*> GetComponents();

    void SetSkybox(std::unique_ptr<TextureCubeMap> cubeMap);
    void RemoveSkybox();

protected:
    friend class Engine;

    void UpdateForDestruction();
    void DeleteAll();

private:
    Components& components;
    FileSystem& fileSystem;

    std::unordered_multimap<std::string, std::unique_ptr<Object>> objects;
};

template<typename T>
T* Objects::CreateObject(const std::string& name)
{
    static_assert(std::is_base_of<Object, T>::value, "T must be derived from Object");
    auto object = std::make_unique<T>(name);
    auto objectRawPtr = object.get();
    objects.insert({ name, std::move(object) });
    objectRawPtr->Initialize();
    return objectRawPtr;
}

template<typename T>
T* Objects::CloneObject(T* object)
{
    static_assert(std::is_base_of<Object, T>::value, "T must be derived from Object");
    std::unique_ptr<Object> clonedObject = std::move(object->Clone<T>());
    auto clonedObjectRawPtr = clonedObject.get();
    objects.insert({ clonedObject->GetName(), std::move(clonedObject) });
    clonedObjectRawPtr->Initialize();

    auto transform = object->GetComponent<Transform>();
    for (int childIndex = 0; childIndex < transform->GetChildCount(); childIndex++)
    {
        auto child = transform->GetChild(childIndex)->GetObject();
        auto clonedChild = CloneObject(child);
        clonedChild->GetComponent<Transform>()->SetParent(clonedObjectRawPtr);
    }

    return clonedObjectRawPtr;
}

template<typename T>
std::vector<T*> Objects::GetObjects()
{
    static_assert(std::is_base_of<Object, T>::value, "T must be derived from Object");

    std::vector<T*> result;
    for (const auto& object : objects)
    {
        if (auto rawptr = dynamic_cast<T*>(object.second.get()))
            result.push_back(rawptr);
    }
    return result;
}

template<typename T>
std::vector<T*> Objects::GetComponents()
{
    static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");

    std::vector<T*> allComponents;
    for (const auto& object : objects)
    {
        auto components = object.second.get()->GetComponents<T>();
        std::move(std::begin(components), std::end(components), std::back_inserter(allComponents));
    }
    return allComponents;
}
