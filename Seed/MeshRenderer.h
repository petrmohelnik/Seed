#pragma once
#include "Renderer.h"

class MeshRenderer : public Renderer
{
public:
    using Renderer::Renderer;

	void Render() override;
};
