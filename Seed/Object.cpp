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
    if(auto parent = transform->GetParent())
        parent->CleanChildren();
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
        if (renderer)
            renderer->Destroy();
        if (camera)
            camera->Destroy();
        if (light)
            light->Destroy();
        for (const auto& audio : audios)
            audio->Destroy();
        for (const auto& collider : colliders)
            collider->Destroy();
        if (rigidbody)
            rigidbody->Destroy();
        for (const auto& script : scripts)
            script->Destroy();

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
    for (const auto& collider : colliders)
        isSomeComponentDestroyed = collider->UpdateForDestruction() || isSomeComponentDestroyed;
    if (rigidbody)
        isSomeComponentDestroyed = rigidbody->UpdateForDestruction() || isSomeComponentDestroyed;
    for (const auto& script : scripts)
        isSomeComponentDestroyed = script->UpdateForDestruction() || isSomeComponentDestroyed;

    return isSomeComponentDestroyed;
}

bool Object::DoDestruction()
{
    if (registeredForDestruction && timeToDestruction <= 0)
        return true;

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
    std::experimental::erase_if(colliders, [](const std::unique_ptr<Collider>& collider)
    {
        return collider->ToBeDestroyed();
    });
    if (rigidbody && rigidbody->ToBeDestroyed())
        rigidbody.reset();
    std::experimental::erase_if(scripts, [](const std::unique_ptr<Script>& script)
    {
        return script->ToBeDestroyed();
    });

    return false;
}
