#include "Components.h"
#include "Input.h"
#include "Time.h"
#include "Object.h"
#include "Engine.h"

Components::Components() 
    : input(Engine::GetInput())
    , time(Engine::GetTime())
    , objects(Engine::GetObjects())
{
}

void Components::OnFrameUpdate()
{
    for (const auto& script : scripts)
    {
        script->Update();
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
