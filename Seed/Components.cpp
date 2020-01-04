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

void Components::OnFixedUpdate()
{
    for (const auto& script : scripts)
    {
        script->FixedUpdate();
    }
}

void Components::OnFrameUpdate()
{
    for (const auto& script : scripts)
    {
        script->Update();
    }
}

void Components::SimulatePhysics()
{
    physicsEngine.RunSimulationStep();
    physicsEngine.OnCollisionUpdate();
    physicsEngine.RigidBodyUpdate();
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
