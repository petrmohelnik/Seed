#include "Renderer.h"
#include "Input.h"
#include "Time.h"

Renderer::Renderer(std::weak_ptr<Object> object,
                   Input& input,
                   Time& time)
    : Component(object)
    , input(input)
    , time(time)
{
}
