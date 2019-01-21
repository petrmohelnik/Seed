#pragma once

class Object;
class Components;
class FileSystem;

class Objects
{
public:
    Objects(Components& components, FileSystem& fileSystem);

	template<typename T>
	T* CreateObject(const std::string& name);
    Object* GetObjectByName(const std::string& name);
    std::vector<Object*> GetObjectsByName(const std::string& name);
    std::vector<Object*> GetObjectsByTag(const std::string& tag);
    template<typename T>
    std::vector<T*> GetObjects();

protected:
    friend class Engine;

    void UpdateForDestruction();

private:
    Components& components;
    FileSystem& fileSystem;

    std::unordered_map<std::string, std::unique_ptr<Object>> objects;
};

template<typename T>
T* Objects::CreateObject(const std::string& name)
{
	static_assert(std::is_base_of<Object, T>::value, "T must be derived from Object");
	auto object = std::make_unique<T>(name, *this, components, fileSystem);
    auto objectRawPtr = object.get();
    objects.insert({ name, std::move(object) });
    objectRawPtr->Initialize();
	return objectRawPtr;
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
