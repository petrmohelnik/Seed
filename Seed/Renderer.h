#pragma once
#include "Component.h"

class Renderer : public Component
{
public:
	virtual ~Renderer() = default;
	virtual void Render() = 0;
};