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
        if(script->GetObject()->IsSelfActive())
            script->FixedUpdate();
    }
}

void Components::OnFrameUpdate()
{
    physicsEngine.OnMouseUpdate();

    for (const auto& script : scripts)
    {
        if (script->GetObject()->IsSelfActive())
            script->Update();
    }
}

void Components::SimulatePhysics()
{
    physicsEngine.RunSimulationStep();
    physicsEngine.OnCollisionUpdate();
    physicsEngine.RigidbodyUpdate();
}

void Components::ActivationUpdate()
{
    root->UpdateActivationInChildren();
}

void Components::Render()
{
    root->GetComponent<Transform>()->UpdateModelMatrix();
    renderingPipeline.Render();
}

void Components::AddComponentsOfObject(Object* object)
{
    if (auto renderer = object->GetComponent<Renderer>())
        renderingPipeline.AddRenderer(renderer);
    if (auto camera = object->GetComponent<Camera>())
        renderingPipeline.AddCamera(camera);
    if (auto light = object->GetComponent<Light>())
        renderingPipeline.AddLight(light);
    for (auto audio : object->GetComponents<Audio>())
        audios.push_back(audio);
    if (auto collider = object->GetComponent<Collider>())
        physicsEngine.AddCollider(collider);
    for (auto script : object->GetComponents<Script>())
        scripts.push_back(script);
}

void Components::CleanComponents()
{
    renderingPipeline.CleanComponents();
    physicsEngine.CleanComponents();

    std::experimental::erase_if(audios, [](const auto audio)
    {
        return audio->ToBeDestroyed() || !audio->GetObject()->IsActive();
    });
    std::experimental::erase_if(scripts, [] (const auto script)
    {
        return script->ToBeDestroyed() || !script->GetObject()->IsActive();
    });
}
