#pragma once
#include "Components.h"

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
	Object(Components& components);
	virtual ~Object() = default;

    template<typename T>
	T* AddRenderer();
    Camera* AddCamera();
    Light* AddLight();
    Audio* AddAudio();
    template<typename T>
    T* AddCollider();
    Rigidbody* AddRigidbody();
    template<typename T>
    T* AddScript();

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

protected:
    virtual void Initialize();

private:
	std::shared_ptr<Transform> transform;
	std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Light> light;
    std::vector<std::shared_ptr<Audio>> audios;
    std::vector<std::shared_ptr<Collider>> colliders;
    std::shared_ptr<Rigidbody> rigidbody;
	std::vector<std::shared_ptr<Script>> scripts;

    Components& components;
};

template<typename T>
T* Object::AddRenderer()
{
    auto rendererT = components.CreateRenderer<T>(weak_from_this());
    renderer = rendererT;
    return rendererT.get();
}

template<typename T>
T* Object::AddCollider()
{
    auto collider = components.CreateCollider<T>(weak_from_this());
    colliders.push_back(collider);
    return collider.get();
}

template<typename T>
T* Object::AddScript()
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
    T* audioRawPointer = nullptr;
    for (const auto& audio : audios)
    {
        if ((audioRawPointer = dynamic_cast<T*>(audio.get())) != nullptr)
            return audioRawPointer;
    }
    return nullptr;
}

template<class T, typename std::enable_if<std::is_base_of<Collider, T>::value>::type*>
inline T* Object::GetComponent()
{
    T* colliderRawPointer = nullptr;
    for (const auto& collider : colliders)
    {
        if ((colliderRawPointer = dynamic_cast<T*>(collider.get())) != nullptr)
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
    T* scriptRawPointer = nullptr;
    for (const auto& script : scripts)
    {
        if ((scriptRawPointer = dynamic_cast<T*>(script.get())) != nullptr)
            return scriptRawPointer;
    }
    return nullptr;
}
