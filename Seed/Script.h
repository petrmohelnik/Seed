#pragma once
#include "Component.h"

class Input;
class Time;
class Objects;
class PhysicsEngine;

struct Collision;

class Script : public Component
{
public:
	Script(Object* object, PhysicsEngine& physics);
	virtual ~Script() = default;
	
	virtual void OnCreate() {};
	virtual void FixedUpdate() {};
	virtual void OnCollisionEnter(Collision const& collision) {};
	virtual void OnCollisionStay(Collision const& collision) {};
	virtual void OnCollisionExit(Collision const& collision) {};
	virtual void Update() {};
	virtual void OnMouseDown() {};
	virtual void OnDestroy() {};

protected:
    Input& input;
    Time& time;
    Objects& objects;
    PhysicsEngine& physics;
    Transform* transform;
};