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

void Components::Initialize()
{
    renderingPipeline.Initialize();
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
    renderingPipeline.Render();
}

void Components::CleanComponents()
{
    renderingPipeline.CleanComponents();
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
