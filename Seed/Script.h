#pragma once
#include "Component.h"

class Input;
class Time;
class Objects;
class PhysicsEngine;

class Script : public Component
{
public:
	Script(Object* object, PhysicsEngine& physics);
	virtual ~Script() = default;
	
	virtual void OnCreate() {};
	virtual void FixedUpdate() {};
	virtual void OnCollisionEnter() {};
	virtual void OnCollisionStay() {};
	virtual void OnCollisionExit() {};
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