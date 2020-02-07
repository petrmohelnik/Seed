#include "Renderer.h"
#include "Input.h"
#include "Time.h"
#include "Engine.h"

void Renderer::SetCastsShadow(bool castsShadow_)
{
    castsShadow = castsShadow_;
}

bool Renderer::CastsShadow()
{
    return castsShadow;
}
