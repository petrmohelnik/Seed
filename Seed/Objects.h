#pragma once
#include "Scene.h"

class Objects
{
public:
	Objects(Scene* scene_);

	template<typename T>
	static std::shared_ptr<T> CreateObject();

private:
	static Scene* scene;
};

template<typename T>
inline std::shared_ptr<T> Objects::CreateObject()
{
	static_assert(std::is_base_of<Object, T>::value, "T must be derived from Object");
	auto object = std::make_shared<T>();
	scene->AddObject(object);
	return object;
}
