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
	
	virtual void Start() {};
	virtual void OnFrameUpdate() {};
	virtual void End() {};

private:
    Input& input;
    Time& time;
    Objects& objects;
    PhysicsEngine& physics;
};