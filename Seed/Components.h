#pragma once
#include "RenderingPipeline.h"
#include "PhysicsSystem.h"
#include "Renderer.h"
#include "Camera.h"
#include "Light.h"
#include "Audio.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Script.h"

class Input;
class Time;
class Objects;

class Components
{
public:
	Components(Input& input,
               Time& time,
               Objects& objects);

protected:
    friend class Scene;
    friend class Object;

    template<typename T>
	std::shared_ptr<T> CreateRenderer(std::weak_ptr<Object> object);
    std::shared_ptr<Camera> CreateCamera(std::weak_ptr<Object> object);
    std::shared_ptr<Light> CreateLight(std::weak_ptr<Object> object);
    std::shared_ptr<Audio> CreateAudio(std::weak_ptr<Object> object);
    template<typename T>
    std::shared_ptr<T> CreateCollider(std::weak_ptr<Object> object);
    std::shared_ptr<Rigidbody> CreateRigidbody(std::weak_ptr<Object> object);
	template<typename T>
	std::shared_ptr<T> CreateScript(std::weak_ptr<Object> object);
	
    void OnFrameUpdate();
    void Render();
    void CleanComponents();

private:
    Input& input;
    Time& time;
    Objects& objects;

    RenderingPipeline pipeline;
    PhysicsSystem physics;
    std::vector<std::weak_ptr<Audio>> audios;
    std::vector<std::weak_ptr<Script>> scripts;
};

template<typename T>
inline std::shared_ptr<T> Components::CreateRenderer(std::weak_ptr<Object> object)
{
	static_assert(std::is_base_of<Renderer, T>::value, "T must be derived from Renderer");
	auto renderer =  std::make_shared<T>(object, input, time);
    pipeline.AddRenderer(renderer);
	return renderer;
}

inline std::shared_ptr<Camera> Components::CreateCamera(std::weak_ptr<Object> object)
{
    auto camera = std::make_shared<Camera>(object);
    pipeline.AddCamera(camera);
    return camera;
}

inline std::shared_ptr<Light> Components::CreateLight(std::weak_ptr<Object> object)
{
    auto light = std::make_shared<Light>(object);
    pipeline.AddLight(light);
    return light;
}

inline std::shared_ptr<Audio> Components::CreateAudio(std::weak_ptr<Object> object)
{
    auto audio = std::make_shared<Audio>(object);
    audios.push_back(audio);
    return audio;
}

template<typename T>
inline std::shared_ptr<T> Components::CreateCollider(std::weak_ptr<Object> object)
{
    static_assert(std::is_base_of<Collider, T>::value, "T must be derived from Collider");
    auto collider = std::make_shared<T>(object);
    physics.AddCollider(collider);
    return collider;
}

inline std::shared_ptr<Rigidbody> Components::CreateRigidbody(std::weak_ptr<Object> object)
{
    auto rigidbody = std::make_shared<Rigidbody>(object);
    physics.AddRigidbody(rigidbody);
    return rigidbody;
}

template<typename T>
inline std::shared_ptr<T> Components::CreateScript(std::weak_ptr<Object> object)
{
	static_assert(std::is_base_of<Script, T>::value, "T must be derived from Script");
	auto script = std::make_shared<T>(object, input, time, objects);
    scripts.push_back(script);
	return script;
}
