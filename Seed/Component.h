#pragma once

class Object;

class Component
{
public:
    Component(Object* object);
	virtual ~Component() = default;

    Object* GetObject();
    void Destroy();
    bool IsRegisteredForDestruction();

protected:
    friend class Object;
    void RegisterForDestruction();

private:
    Object* object;
    bool toBeDestroyed = false;
};
