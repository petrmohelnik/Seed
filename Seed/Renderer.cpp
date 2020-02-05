#include "Renderer.h"
#include "Input.h"
#include "Time.h"
#include "Engine.h"

void Renderer::SetCastShadow(bool castShadow_)
{
    castShadow = castShadow_;
}

bool Renderer::GetCastShadow()
{
    return castShadow;
}
