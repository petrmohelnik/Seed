#pragma once
#include "Components.h"

class Object;

class Objects
{
public:
    Objects(Components& components);

	template<typename T>
	std::shared_ptr<T> CreateObject();

private:
    Components& components;

    std::vector<std::shared_ptr<Object>> objects;
};

template<typename T>
inline std::shared_ptr<T> Objects::CreateObject()
{
	static_assert(std::is_base_of<Object, T>::value, "T must be derived from Object");
	auto object = std::make_shared<T>(components);
    objects.push_back(object);
    object->Initialize();
	return object;
}
