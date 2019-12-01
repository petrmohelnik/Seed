#include "Renderer.h"
#include "Input.h"
#include "Time.h"
#include "Engine.h"

Renderer::Renderer(Object* object)
    : Component(object)
    , input(Engine::GetInput())
    , time(Engine::GetTime())
{
}

void Renderer::SetCastShadow(bool castShadow_)
{
    castShadow = castShadow_;
}

bool Renderer::GetCastShadow()
{
    return castShadow;
}
