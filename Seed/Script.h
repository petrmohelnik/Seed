#pragma once
#include "Component.h"

class Script : public Component
{
public:
	virtual ~Script() = default;
	virtual void Start() {};
	virtual void OnFrameUpdate() {};
	virtual void End() {};
};