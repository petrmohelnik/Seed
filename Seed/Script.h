#pragma once
#include "Component.h"

class Input;
class Time;
class Objects;

class Script : public Component
{
public:
	Script(Input& input,
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