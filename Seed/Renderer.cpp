#include "Renderer.h"
#include "Input.h"
#include "Time.h"

Renderer::Renderer(Input& input,
                   Time& time)
    : input(input)
    , time(time)
{
}
