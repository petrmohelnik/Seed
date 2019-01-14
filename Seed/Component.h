#pragma once

class Object;

class Component
{
public:
    Component(Object* object);
	virtual ~Component() = default;

    Object* GetObject();
    void Destroy(Uint32 delay = 0);

protected:
    friend class Object;
    friend class Components;
    friend class RenderingPipeline;
    friend class PhysicsEngine;
    bool UpdateForDestruction();
    bool ToBeDestroyed();

private:
    Object* object;
    Sint32 timeToDestruction = SDL_MAX_SINT32;
    bool registeredForDestruction = false;
};
