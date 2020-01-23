#include "Object.h"
#include "Engine.h"
#include "Transform.h"
#include "Renderer.h"
#include "Audio.h"
#include "Script.h"
#include "Time.h"
#include "FileSystem.h"
#include "Time.h"

Object::Object()
    : objects(Engine::GetObjects())
    , components(Engine::GetComponents())
    , fileSystem(Engine::GetFileSystem())
    , time(Engine::GetTime())
{
    transform = std::make_unique<Transform>(this);
    name = "root";
}

Object::Object(std::string name_)
    : objects(Engine::GetObjects())
    , components(Engine::GetComponents())
    , fileSystem(Engine::GetFileSystem())
    , time(Engine::GetTime())
{
    transform = std::make_unique<Transform>(this, components.GetRoot()->GetComponent<Transform>());
    name = std::move(name_);
}

Object::~Object()
{
}

void Object::Destroy(float delay)
{
    if(timeToDestruction >= delay)
        timeToDestruction = delay;
    registeredForDestruction = true;
}

void Object::Initialize()
{
}

bool Object::UpdateForDestruction()
{
    if(registeredForDestruction)
        timeToDestruction -= Engine::GetTime().DeltaTime();

    if (registeredForDestruction && timeToDestruction <= 0)
    {
        transform->DestroyAllChildren();
        transform->Destroy();
        transform->DisconnectChildrenFromParent();

        if (renderer)
            renderer->Destroy();
        if (camera)
            camera->Destroy();
        if (light)
            light->Destroy();
        for (const auto& audio : audios)
            audio->Destroy();
        if (collider)
            collider->Destroy();
        for (const auto& script : scripts)
        {
            script->OnDestroy();
            script->Destroy();
        }

        return true;
    }

    bool isSomeComponentDestroyed = false;
    if (renderer)
        isSomeComponentDestroyed = renderer->UpdateForDestruction() || isSomeComponentDestroyed;
    if (camera)
        isSomeComponentDestroyed = camera->UpdateForDestruction() || isSomeComponentDestroyed;
    if (light)
        isSomeComponentDestroyed = light->UpdateForDestruction() || isSomeComponentDestroyed;
    for (const auto& audio : audios)
        isSomeComponentDestroyed = audio->UpdateForDestruction() || isSomeComponentDestroyed;
    if (collider)
        isSomeComponentDestroyed = collider->UpdateForDestruction() || isSomeComponentDestroyed;
    for (const auto& script : scripts)
        isSomeComponentDestroyed = script->UpdateForDestruction() || isSomeComponentDestroyed;

    return isSomeComponentDestroyed;
}

bool Object::DoDestruction()
{
    if (registeredForDestruction && timeToDestruction <= 0)
    {
        auto parent = transform->GetParent();
        if (parent)
            parent->CleanChildren();
        else if (auto root = components.GetRoot())
            root->GetComponent<Transform>()->CleanChildren();

        return true;
    }

    if (renderer && renderer->ToBeDestroyed())
        renderer.reset();
    if (camera && camera->ToBeDestroyed())
        camera.reset();
    if (light && light->ToBeDestroyed())
        light.reset();
    std::experimental::erase_if(audios, [](const std::unique_ptr<Audio>& audio)
    {
        return audio->ToBeDestroyed();
    });
    if (collider && collider->ToBeDestroyed())
        collider.reset();
    std::experimental::erase_if(scripts, [](const std::unique_ptr<Script>& script)
    {
        return script->ToBeDestroyed();
    });

    return false;
}

std::vector<Component*> Object::GetAllComponents()
{
    std::vector<Component*> components;

    components.push_back(transform.get());
    if(renderer.get() != nullptr)
        components.push_back(renderer.get());
    if(camera.get() != nullptr)
        components.push_back(camera.get());
    if(light.get() != nullptr)
        components.push_back(light.get());

    for (const auto& audio : audios)
        components.push_back(audio.get());
    if (collider.get() != nullptr)
        components.push_back(collider.get());
    for (const auto& script : scripts)
        components.push_back(script.get());

    return components;
}
