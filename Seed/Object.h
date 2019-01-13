#pragma once
#include "Components.h"
#include "Objects.h"

class Transform;
class Renderer;
class Camera;
class Light;
class Audio;
class Collider;
class Rigidbody;
class Script;

class Object : public std::enable_shared_from_this<Object>
{
public:
	Object(std::string name, Objects& objects, Components& components);
	virtual ~Object() = default;

    template <class T, typename std::enable_if<std::is_base_of<Renderer, T>::value>::type* = nullptr>
    T* AddComponent();
    template <class T, typename std::enable_if<std::is_base_of<Camera, T>::value>::type* = nullptr>
    T* AddComponent();
    template <class T, typename std::enable_if<std::is_base_of<Light, T>::value>::type* = nullptr>
    T* AddComponent();
    template <class T, typename std::enable_if<std::is_base_of<Audio, T>::value>::type* = nullptr>
    T* AddComponent();
    template <class T, typename std::enable_if<std::is_base_of<Collider, T>::value>::type* = nullptr>
    T* AddComponent();
    template <class T, typename std::enable_if<std::is_base_of<Rigidbody, T>::value>::type* = nullptr>
    T* AddComponent();
    template <class T, typename std::enable_if<std::is_base_of<Script, T>::value>::type* = nullptr>
    T* AddComponent();

    template <class T, typename std::enable_if<std::is_base_of<Transform, T>::value>::type* = nullptr>
    T* GetComponent();
    template <class T, typename std::enable_if<std::is_base_of<Renderer, T>::value>::type* = nullptr>
    T* GetComponent();
    template <class T, typename std::enable_if<std::is_base_of<Camera, T>::value>::type* = nullptr>
    T* GetComponent();
    template <class T, typename std::enable_if<std::is_base_of<Light, T>::value>::type* = nullptr>
    T* GetComponent();
    template <class T, typename std::enable_if<std::is_base_of<Audio, T>::value>::type* = nullptr>
    T* GetComponent();
    template <class T, typename std::enable_if<std::is_base_of<Collider, T>::value>::type* = nullptr>
    T* GetComponent();
    template <class T, typename std::enable_if<std::is_base_of<Rigidbody, T>::value>::type* = nullptr>
    T* GetComponent();
    template <class T, typename std::enable_if<std::is_base_of<Script, T>::value>::type* = nullptr>
    T* GetComponent();

    Objects::UniqueId GetUniqueId();
    std::string GetName();
    void AddTag(const std::string& tag);
    bool ContainsTag(const std::string& tag);
    void Destroy();

protected:
    friend class Component;
    friend class Objects;

    virtual void Initialize();
    void RegisterForComponentDestruction();
    void DestroyComponents();

private:
	std::shared_ptr<Transform> transform;
	std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Light> light;
    std::vector<std::shared_ptr<Audio>> audios;
    std::vector<std::shared_ptr<Collider>> colliders;
    std::shared_ptr<Rigidbody> rigidbody;
	std::vector<std::shared_ptr<Script>> scripts;

    Objects::UniqueId id;
    std::string name;
    std::unordered_set<std::string> tags;
    Objects& objects;
    Components& components;
};

template<class T, typename std::enable_if<std::is_base_of<Renderer, T>::value>::type*>
inline T * Object::AddComponent()
{
    if (renderer)
        throw std::runtime_error("Object: " + name + "already contains Renderer");
    auto rendererT = components.CreateRenderer<T>(weak_from_this());
    renderer = rendererT;
    return rendererT.get();
}

template<class T, typename std::enable_if<std::is_base_of<Camera, T>::value>::type*>
inline T * Object::AddComponent()
{
    if (camera)
        throw std::runtime_error("Object: " + name + "already contains Camera");
    camera = components.CreateCamera(weak_from_this());
    return camera.get();
}

template<class T, typename std::enable_if<std::is_base_of<Light, T>::value>::type*>
inline T * Object::AddComponent()
{
    if (light)
        throw std::runtime_error("Object: " + name + "already contains Light");
    light = components.CreateLight(weak_from_this());
    return light.get();
}

template<class T, typename std::enable_if<std::is_base_of<Audio, T>::value>::type*>
inline T * Object::AddComponent()
{
    auto audio = components.CreateAudio(weak_from_this());
    audios.push_back(audio);
    return audio.get();
}

template<class T, typename std::enable_if<std::is_base_of<Collider, T>::value>::type*>
inline T * Object::AddComponent()
{
    auto collider = components.CreateCollider<T>(weak_from_this());
    colliders.push_back(collider);
    return collider.get();
}

template<class T, typename std::enable_if<std::is_base_of<Rigidbody, T>::value>::type*>
inline T * Object::AddComponent()
{
    if (rigidbody)
        throw std::runtime_error("Object: " + name + "already contains Rigidbody");
    rigidbody = components.CreateRigidbody(weak_from_this());
    return rigidbody.get();
}

template<class T, typename std::enable_if<std::is_base_of<Script, T>::value>::type*>
inline T * Object::AddComponent()
{
    auto script = components.CreateScript<T>(weak_from_this());
    scripts.push_back(script);
    return script.get();
}

template<class T, typename std::enable_if<std::is_base_of<Transform, T>::value>::type*>
inline T* Object::GetComponent()
{
    return transform.get();
}

template<class T, typename std::enable_if<std::is_base_of<Renderer, T>::value>::type*>
inline T* Object::GetComponent()
{
    return dynamic_cast<T*>(renderer.get());
}

template<class T, typename std::enable_if<std::is_base_of<Camera, T>::value>::type*>
inline T* Object::GetComponent()
{
    return camera.get();
}

template<class T, typename std::enable_if<std::is_base_of<Light, T>::value>::type*>
inline T* Object::GetComponent()
{
    return light.get();
}

template<class T, typename std::enable_if<std::is_base_of<Audio, T>::value>::type*>
inline T* Object::GetComponent()
{
    for (const auto& audio : audios)
    {
        if (auto audioRawPointer = dynamic_cast<T*>(audio.get()))
            return audioRawPointer;
    }
    return nullptr;
}

template<class T, typename std::enable_if<std::is_base_of<Collider, T>::value>::type*>
inline T* Object::GetComponent()
{
    for (const auto& collider : colliders)
    {
        if (auto colliderRawPointer = dynamic_cast<T*>(collider.get()))
            return colliderRawPointer;
    }
    return nullptr;
}

template<class T, typename std::enable_if<std::is_base_of<Rigidbody, T>::value>::type*>
inline T* Object::GetComponent()
{
    return rigidbody.get();
}

template<class T, typename std::enable_if<std::is_base_of<Script, T>::value>::type*>
inline T* Object::GetComponent()
{
    for (const auto& script : scripts)
    {
        if (auto scriptRawPointer = dynamic_cast<T*>(script.get()))
            return scriptRawPointer;
    }
    return nullptr;
}
