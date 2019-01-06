#pragma once
#include "Renderer.h"

class MeshRenderer : public Renderer
{
public:
	MeshRenderer();

	void Render() override;
};
