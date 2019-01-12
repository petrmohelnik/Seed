#pragma once
#include "Component.h"

class Object;
class Input;
class Time;
class Objects;

class Script : public Component
{
public:
	Script(std::weak_ptr<Object> object,
           Input& input,
           Time& time,
	       Objects& objects);
	virtual ~Script() = default;
	
	virtual void Start() {};
	virtual void OnFrameUpdate() {};
	virtual void End() {};

private:
    Input& input;
    Time& time;
    Objects& objects;
};