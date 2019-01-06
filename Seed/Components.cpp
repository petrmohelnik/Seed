#include "Components.h"
#include "Input.h"
#include "Time.h"
#include "Object.h"

Components::Components(Input& input,
                       Time& time,
                       Objects& objects) 
    : input(input)
    , time(time)
    , objects(objects)
{
}

void Components::OnFrameUpdate()
{
    for (const auto& script : scripts)
    {
        script.lock()->OnFrameUpdate();
    }
}

void Components::Render()
{
    for (const auto& renderer : renderers)
    {
        renderer.lock()->Render();
    }
}

void Components::CleanComponents()
{
    std::remove_if(std::begin(scripts), std::end(scripts), [] (const auto& script)
    {
        return script.expired();
    });

    std::remove_if(std::begin(renderers), std::end(renderers), [] (const auto& renderer)
    {
        return renderer.expired();
    });
}
