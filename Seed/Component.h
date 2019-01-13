#pragma once

class Object;

class Component
{
public:
    Component(std::weak_ptr<Object> object);
	virtual ~Component() = default;

	std::shared_ptr<Object> GetObject();
    void Destroy();
    bool IsRegisteredForDestruction();

private:
	std::weak_ptr<Object> object;
    bool toBeDestroyed = false;
};
