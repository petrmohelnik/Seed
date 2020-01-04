#pragma once
#include "Components.h"
#include "Objects.h"
#include "Identifiable.h"

class Transform;
class Renderer;
class Camera;
class Light;
class Audio;
class Collider;
class Rigidbody;
class Script;
class FileSystem;
class Time;

class Object : public Identifiable
{
public:
	Object();
	Object(std::string name_);
    virtual ~Object();
    Object(Object&& m) = delete;
    Object(const Object& m) = delete;
    Object& operator=(const Object& m) = delete;
    Object& operator=(Object&& m) = delete;

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

    void Destroy(float delay = 0);

protected:
    friend class Component;
    friend class Objects;
    friend class Transform;
    friend class Input;

    virtual void Initialize();
    bool UpdateForDestruction();
    bool DoDestruction();
    std::vector<Component*> GetAllComponents();

private:
	std::unique_ptr<Transform> transform;
	std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Light> light;
    std::vector<std::unique_ptr<Audio>> audios;
    std::vector<std::unique_ptr<Collider>> colliders;
    std::unique_ptr<Rigidbody> rigidbody;
	std::vector<std::unique_ptr<Script>> scripts;

    Objects& objects;
    Components& components;
    FileSystem& fileSystem;
    Time& time;
    float timeToDestruction = std::numeric_limits<float>::max();
    bool registeredForDestruction = false;
};

template<class T, typename std::enable_if<std::is_base_of<Renderer, T>::value>::type*>
inline T* Object::AddComponent()
{
    if (renderer)
        throw std::runtime_error("Object: " + name + "already contains Renderer");
    renderer = components.CreateRenderer<T>(this);
    return dynamic_cast<T*>(renderer.get());
}

template<class T, typename std::enable_if<std::is_base_of<Camera, T>::value>::type*>
inline T* Object::AddComponent()
{
    if (camera)
        throw std::runtime_error("Object: " + name + "already contains Camera");
    camera = components.CreateCamera(this);
    return camera.get();
}

template<class T, typename std::enable_if<std::is_base_of<Light, T>::value>::type*>
inline T* Object::AddComponent()
{
    if (light)
        throw std::runtime_error("Object: " + name + "already contains Light");
    light = components.CreateLight(this);
    return light.get();
}

template<class T, typename std::enable_if<std::is_base_of<Audio, T>::value>::type*>
inline T* Object::AddComponent()
{
    audios.push_back(components.CreateAudio(this));
    return audios.back().get();
}

template<class T, typename std::enable_if<std::is_base_of<Collider, T>::value>::type*>
inline T* Object::AddComponent()
{
    colliders.push_back(components.CreateCollider<T>(this));
    return dynamic_cast<T*>(colliders.back().get());
}

template<class T, typename std::enable_if<std::is_base_of<Rigidbody, T>::value>::type*>
inline T* Object::AddComponent()
{
    if (rigidbody)
        throw std::runtime_error("Object: " + name + "already contains Rigidbody");
    rigidbody = components.CreateRigidbody(this);
    return rigidbody.get();
}

template<class T, typename std::enable_if<std::is_base_of<Script, T>::value>::type*>
inline T* Object::AddComponent()
{
    scripts.push_back(components.CreateScript<T>(this));
    return dynamic_cast<T*>(scripts.back().get());
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
