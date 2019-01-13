#include "Renderer.h"
#include "Input.h"
#include "Time.h"

Renderer::Renderer(Object* object,
                   Input& input,
                   Time& time)
    : Component(object)
    , input(input)
    , time(time)
{
}
