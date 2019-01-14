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
        script->OnFrameUpdate();
    }
}

void Components::Render()
{
    pipeline.Render();
}

void Components::CleanComponents()
{
    pipeline.CleanComponents();
    physics.CleanComponents();

    std::experimental::erase_if(audios, [](const auto audio)
    {
        return audio->ToBeDestroyed();
    });
    std::experimental::erase_if(scripts, [] (const auto script)
    {
        return script->ToBeDestroyed();
    });
}
