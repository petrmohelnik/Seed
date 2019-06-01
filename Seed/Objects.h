#pragma once

class Object;
class Components;
class FileSystem;
class TextureCubeMap;

class Objects
{
public:
    Objects();

	template<typename T = Object>
	T* CreateObject(const std::string& name);
	Object* CreateObjectWithMesh(const std::string& name, const std::string& meshFilePath);
    Object* GetObjectByName(const std::string& name);
    std::vector<Object*> GetObjectsByName(const std::string& name);
    std::vector<Object*> GetObjectsByTag(const std::string& tag);
    template<typename T>
    std::vector<T*> GetObjects();

    void SetSkybox(std::shared_ptr<TextureCubeMap> cubeMap);
	void RemoveSkybox();

protected:
    friend class Engine;

    void UpdateForDestruction();

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
