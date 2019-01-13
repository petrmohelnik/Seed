#pragma once

class Object;
class Components;

class Objects
{
public:
    Objects(Components& components);

	template<typename T>
	T* CreateObject(const std::string& name);
    Object* GetObjectByName(const std::string& name);
    std::vector<Object*> GetObjectsByName(const std::string& name);
    std::vector<Object*> GetObjectsByTag(const std::string& tag);
    template<typename T>
    std::vector<T*> GetObjects();

    using UniqueId = unsigned long;

protected:
    friend class Object;
    friend class Scene;

    void RegisterForDestruction(UniqueId uniqueId);
    void RegisterForComponentDestruction(UniqueId uniqueId);
    UniqueId CreateUniqueId();
    void Destroy();

private:
    Components& components;

    std::unordered_map<UniqueId, std::shared_ptr<Object>> objects;
    std::set<UniqueId> objectsToBeDestroyed;
    std::set<UniqueId> objectsToDestroyComponents;

    static UniqueId uniqueId;
};

template<typename T>
T* Objects::CreateObject(const std::string& name)
{
	static_assert(std::is_base_of<Object, T>::value, "T must be derived from Object");
	auto object = std::make_shared<T>(name, *this, components);
    objects.insert({ object->GetUniqueId(), object });
    object->Initialize();
	return object.get();
}

template<typename T>
std::vector<T*> Objects::GetObjects()
{
    static_assert(std::is_base_of<Object, T>::value, "T must be derived from Object");
    std::vector<T*> result;
    T* rawptr = nullptr;
    for (const auto& object : objects)
    {
        if ((rawptr = dynamic_cast<T*>(object.second.get())) != nullptr)
            result.push_back(rawptr);
    }
    return result;
}
