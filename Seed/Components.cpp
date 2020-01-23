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
    root = std::make_unique<Object>();
    root->GetComponent<Transform>()->MakeRoot();
    renderingPipeline.SetRootTransform(root->GetComponent<Transform>());
}

Components::~Components()
{
    root.reset(nullptr);
}

Object* Components::GetRoot()
{
    return root.get();;
}

void Components::Initialize()
{
    renderingPipeline.Initialize();
    physicsEngine.Initialize();
}

void Components::SetSkybox(std::unique_ptr<Skybox> skybox_)
{
    skybox = std::move(skybox_);
    renderingPipeline.SetSkybox(skybox.get());
}

void Components::RemoveSkybox()
{
    skybox.reset();
    renderingPipeline.SetSkybox(nullptr);
}

void Components::OnCreateUpdate()
{
    for (const auto& script : scripts)
    {
        if (!script->initialized)
        {
            script->OnCreate();
            script->initialized = true;
        }
    }
}

void Components::OnFixedUpdate()
{
    for (const auto& script : scripts)
    {
        script->FixedUpdate();
    }
}

void Components::OnFrameUpdate()
{
    physicsEngine.OnMouseUpdate();

    for (const auto& script : scripts)
    {
        script->Update();
    }
}

void Components::SimulatePhysics()
{
    physicsEngine.RunSimulationStep();
    physicsEngine.OnCollisionUpdate();
    physicsEngine.RigidbodyUpdate();
}

void Components::Render()
{
    root->GetComponent<Transform>()->UpdateModelMatrix();
    renderingPipeline.Render();
}

void Components::CleanComponents()
{
    renderingPipeline.CleanComponents();
    physicsEngine.CleanComponents();

    std::experimental::erase_if(audios, [](const auto audio)
    {
        return audio->ToBeDestroyed();
    });
    std::experimental::erase_if(scripts, [] (const auto script)
    {
        return script->ToBeDestroyed();
    });
}
