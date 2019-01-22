#pragma once
#include "Component.h"

class Input;
class Time;
class Objects;
class PhysicsEngine;

class Script : public Component
{
public:
	Script(Object* object,
           Input& input,
           Time& time,
	       Objects& objects,
           PhysicsEngine& physics);
	virtual ~Script() = default;
	
	virtual void OnCreate() {};
	virtual void FixedUpdate() {};
	virtual void OnTrigger() {};
	virtual void OnCollision() {};
	virtual void Update() {};
	virtual void OnMouseDown() {};
	virtual void OnDestroy() {};

private:
    Input& input;
    Time& time;
    Objects& objects;
    PhysicsEngine& physics;
};