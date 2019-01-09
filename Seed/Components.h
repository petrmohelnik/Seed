#pragma once
#include "RenderingPipeline.h"
#include "Renderer.h"
#include "Camera.h"
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

    template<typename T>
	std::shared_ptr<T> CreateRenderer();
    std::shared_ptr<Camera> CreateCamera();
	template<typename T>
	std::shared_ptr<T> CreateScript();

protected:
    friend class Scene;
	
    void OnFrameUpdate();
    void Render();
    void CleanComponents();

private:
    Input& input;
    Time& time;
    Objects& objects;

    RenderingPipeline pipeline;
    std::vector<std::weak_ptr<Script>> scripts;
};

template<typename T>
inline std::shared_ptr<T> Components::CreateRenderer()
{
	static_assert(std::is_base_of<Renderer, T>::value, "T must be derived from Renderer");
	auto renderer =  std::make_shared<T>(input, time);
    pipeline.AddRenderer(renderer);
	return renderer;
}

inline std::shared_ptr<Camera> Components::CreateCamera()
{
    auto camera = std::make_shared<Camera>();
    pipeline.AddCamera(camera);
    return camera;
}

template<typename T>
inline std::shared_ptr<T> Components::CreateScript()
{
	static_assert(std::is_base_of<Script, T>::value, "T must be derived from Script");
	auto script = std::make_shared<T>(input, time, objects);
    scripts.push_back(script);
	return script;
}
