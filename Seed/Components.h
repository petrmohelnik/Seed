#pragma once
#include "RenderingPipeline.h"
#include "PhysicsEngine.h"
#include "Renderer.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "Audio.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "CapsuleCollider.h"
#include "MeshCollider.h"
#include "SphereCollider.h"
#include "CharacterController.h"
#include "Script.h"
#include "Skybox.h"
#include "Texture.h"
#include "Framebuffer.h"

class Input;
class Time;
class Objects;

class Components
{
public:
    Components();
    ~Components();

protected:
    friend class Objects;
    friend class Object;
    friend class Scene;
    friend class Engine;
    friend class Input;

    void Initialize();

    template<typename T>
    std::unique_ptr<T> CreateRenderer(Object* object);
    std::unique_ptr<Camera> CreateCamera(Object* object);
    std::unique_ptr<Light> CreateLight(Object* object);
    std::unique_ptr<Audio> CreateAudio(Object* object);
    template<typename T, typename ...Args>
    std::unique_ptr<T> CreateCollider(Object* object, Args&& ...arguments);
    template<typename T>
    std::unique_ptr<T> CreateScript(Object* object);

    void SetSkybox(std::unique_ptr<Skybox> skybox);
    void RemoveSkybox();
    
    void OnCreateUpdate();
    void OnFixedUpdate();
    void OnFrameUpdate();
    void SimulatePhysics();
    void ActivationUpdate();
    void Render();
    void AddComponentsOfObject(Object* object);
    void CleanComponents();
    Object* GetRoot();

private:
    Input& input;
    Time& time;
    Objects& objects;

    RenderingPipeline renderingPipeline;
    PhysicsEngine physicsEngine;
    std::vector<Audio*> audios;
    std::vector<Script*> scripts;
    std::unique_ptr<Skybox> skybox;
    std::unique_ptr<Object> root;
};

template<typename T>
inline std::unique_ptr<T> Components::CreateRenderer(Object* object)
{
    static_assert(std::is_base_of<Renderer, T>::value, "T must be derived from Renderer");
    auto renderer =  std::make_unique<T>(object);
    renderingPipeline.AddRenderer(renderer.get());
    return renderer;
}

inline std::unique_ptr<Camera> Components::CreateCamera(Object* object)
{
    auto camera = std::make_unique<Camera>(object);
    renderingPipeline.AddCamera(camera.get());
    return camera;
}

inline std::unique_ptr<Light> Components::CreateLight(Object* object)
{
    auto light = std::make_unique<Light>(object);
    renderingPipeline.AddLight(light.get());
    return light;
}

inline std::unique_ptr<Audio> Components::CreateAudio(Object* object)
{
    auto audio = std::make_unique<Audio>(object);
    audios.push_back(audio.get());
    return audio;
}

template<typename T, typename ...Args>
inline std::unique_ptr<T> Components::CreateCollider(Object* object, Args&& ...arguments)
{
    static_assert(std::is_base_of<Collider, T>::value, "T must be derived from Collider");
    auto collider = std::make_unique<T>(object, std::forward<Args>(arguments)...);
    physicsEngine.AddCollider(collider.get());
    return collider;
}

template<typename T>
inline std::unique_ptr<T> Components::CreateScript(Object* object)
{
    static_assert(std::is_base_of<Script, T>::value, "T must be derived from Script");
    auto script = std::make_unique<T>(object, physicsEngine);
    scripts.push_back(script.get());
    return script;
}
