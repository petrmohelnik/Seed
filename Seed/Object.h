#pragma once
#include "Components.h"
#include "Identifiable.h"

class Transform;
class Renderer;
class Camera;
class Light;
class Audio;
class Collider;
class Script;
class FileSystem;
class Time;
class Objects;

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
    template <class T, typename ...Args, typename std::enable_if<std::is_base_of<Collider, T>::value>::type* = nullptr>
    T* AddComponent(Args&& ...arguments);
    template <class T, typename std::enable_if<std::is_base_of<Script, T>::value>::type* = nullptr>
    T* AddComponent();
    template <typename T>
    T* AddComponent(T* componentSource);

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
    template <class T, typename std::enable_if<std::is_base_of<Script, T>::value>::type* = nullptr>
    T* GetComponent();

    template <class T>
    std::vector<T*> GetComponents();
    std::vector<Object*> GetAllChildren();

    void Destroy(float delay = 0);

    void SetActive(bool active);
    bool IsActive();
    bool IsSelfActive();

protected:
    friend class Component;
    friend class Components;
    friend class Objects;
    friend class Transform;
    friend class Input;
    friend class PhysicsEngine;

    virtual void Initialize();
    bool UpdateForDestruction();
    bool DoDestruction();
    std::vector<Component*> GetAllComponents();
    void UpdateActivationInChildren();
    void GetAllChildren(std::vector<Object*>& children);
    template <class T>
    std::unique_ptr<T> Clone();

    Objects& objects;

private:
    std::unique_ptr<Transform> transform;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Light> light;
    std::vector<std::unique_ptr<Audio>> audios;
    std::unique_ptr<Collider> collider;
    std::vector<std::unique_ptr<Script>> scripts;

    Components& components;
    FileSystem& fileSystem;
    Time& time;
    float timeToDestruction = std::numeric_limits<float>::max();
    bool registeredForDestruction = false;

    bool isActive = true;
    bool selfActive = true;
    bool isActiveDirty = false;
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

template<class T, typename ...Args, typename std::enable_if<std::is_base_of<Collider, T>::value>::type*>
inline T* Object::AddComponent(Args&& ...arguments)
{
    if (collider)
        throw std::runtime_error("Object: " + name + "already contains Collider");
    collider = components.CreateCollider<T>(this, std::forward<Args>(arguments)...);
    return dynamic_cast<T*>(collider.get());
}

template<class T, typename std::enable_if<std::is_base_of<Script, T>::value>::type*>
inline T* Object::AddComponent()
{
    scripts.push_back(components.CreateScript<T>(this));
    return dynamic_cast<T*>(scripts.back().get());
}

template<class T>
inline std::unique_ptr<T> Object::Clone()
{
    auto object = std::make_unique<T>(GetName());

    object->transform->Clone(transform.get());
    if (renderer)
        object->AddComponent(renderer.get());
    if (camera)
        object->AddComponent(camera.get());
    if (light)
        object->AddComponent(light.get());
    for (const auto& audio : audios)
        object->AddComponent(audio.get());
    if (collider)
        object->AddComponent(collider.get());
    for (const auto& script : scripts)
        object->AddComponent(script.get());

    object->selfActive = selfActive;
    object->isActiveDirty = isActiveDirty;

    return object;
}

template<typename T>
inline T* Object::AddComponent(T* componentSource)
{
    static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");

    T* clonedComponent = nullptr;
    if constexpr (std::is_base_of<Renderer, T>::value)
    {
        if (renderer)
            throw std::runtime_error("Object: " + name + "already contains Renderer");
        renderer = components.CloneRenderer<T>(componentSource);
        clonedComponent = dynamic_cast<T*>(renderer.get());
    }
    else if constexpr (std::is_base_of<Camera, T>::value)
    {
        if (camera)
            throw std::runtime_error("Object: " + name + "already contains Camera");
        camera = components.CloneCamera(componentSource);
        clonedComponent = camera.get();
    }
    else if constexpr (std::is_base_of<Light, T>::value)
    {
        if (light)
            throw std::runtime_error("Object: " + name + "already contains Light");
        light = components.CloneLight(componentSource);
        clonedComponent = light.get();
    }
    else if constexpr (std::is_base_of<Audio, T>::value)
    {
        audios.push_back(components.CloneAudio(componentSource));
        clonedComponent = audios.back().get();
    }
    else if constexpr (std::is_base_of<Collider, T>::value)
    {
        if (collider)
            throw std::runtime_error("Object: " + name + "already contains Collider");
        collider = components.CloneCollider<T>(componentSource);
        clonedComponent = dynamic_cast<T*>(collider.get());
    }
    else if constexpr (std::is_base_of<Script, T>::value)
    {
        scripts.push_back(components.CloneScript<T>(componentSource));
        clonedComponent = dynamic_cast<T*>(scripts.back().get());
        clonedComponent->transform = GetComponent<Transform>();
    }
    else
    {
        static_assert("component type does not support cloning");
    }

    clonedComponent->object = this;
    return clonedComponent;
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
    return dynamic_cast<T*>(collider.get());
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

template<class T>
inline std::vector<T*> Object::GetComponents()
{
    std::vector<T*> components;

    if constexpr (std::is_base_of<Script, T>::value)
    {
        for (const auto& script : scripts)
        {
            if (auto scriptRawPointer = dynamic_cast<T*>(script.get()))
                components.push_back(scriptRawPointer);
        }
    }
    else if constexpr (std::is_base_of<Audio, T>::value)
    {
        for (const auto& audio : audios)
        {
            if (auto audioRawPointer = dynamic_cast<T*>(audio.get()))
                components.push_back(audioRawPointer);
        }
    }
    else
    {
        if (auto rawPointer = GetComponent<T>())
            components.push_back(rawPointer);
    }

    return components;
}
