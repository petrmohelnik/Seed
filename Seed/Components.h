#pragma once
#include "Renderer.h"
#include "Script.h"
#include "Scene.h"

class Components
{
public:
	Components(Scene* scene_);

	template<typename T>
	static std::shared_ptr<T> CreateRenderer();
	template<typename T>
	static std::shared_ptr<T> CreateScript();

private:
	static Scene* scene;
};

template<typename T>
inline std::shared_ptr<T> Components::CreateRenderer()
{
	static_assert(std::is_base_of<Renderer, T>::value, "T must be derived from Renderer");
	auto renderer =  std::make_shared<T>();
	scene->RegisterRenderer(renderer);
	return renderer;
}

template<typename T>
inline std::shared_ptr<T> Components::CreateScript()
{
	static_assert(std::is_base_of<Script, T>::value, "T must be derived from Script");
	auto script = std::make_shared<T>();
	scene->RegisterScript(script);
	return script;
}
