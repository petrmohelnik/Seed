#pragma once
#include "Scene.h"

class Object;

class Objects
{
public:
	template<typename T>
	std::shared_ptr<T> CreateObject();

private:
    std::vector<std::shared_ptr<Object>> objects;
};

template<typename T>
inline std::shared_ptr<T> Objects::CreateObject()
{
	static_assert(std::is_base_of<Object, T>::value, "T must be derived from Object");
	auto object = std::make_shared<T>();
    objects.push_back(object);
	return object;
}
