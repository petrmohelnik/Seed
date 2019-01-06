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
    pipeline.Render();
}

void Components::CleanComponents()
{
    pipeline.CleanRenderers();

    std::remove_if(std::begin(scripts), std::end(scripts), [] (const auto& script)
    {
        return script.expired();
    });
}
