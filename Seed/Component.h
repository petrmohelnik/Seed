#pragma once
#include "Identifiable.h"

class Object;
class Transform;

class Component : public Identifiable
{
public:
    Component(Object* object);
	virtual ~Component() = default;

    Object* GetObject();
    Transform* GetTransform();
    void Destroy(float delay = 0);

protected:
    friend class Object;
    friend class Components;
    friend class RenderingPipeline;
    friend class PhysicsEngine;
    bool UpdateForDestruction();
    bool ToBeDestroyed();

private:
    Object* object;
    float timeToDestruction = std::numeric_limits<float>::max();
    bool registeredForDestruction = false;
};
