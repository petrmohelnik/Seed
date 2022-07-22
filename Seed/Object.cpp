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

std::vector<Object*> Object::GetAllChildren()
{
    std::vector<Object*> children;

    GetAllChildren(children);

    return children;
}

void Object::GetAllChildren(std::vector<Object*>& children)
{
    auto transform = GetComponent<Transform>();
    for (int childIndex = 0; childIndex < transform->GetChildCount(); childIndex++)
    {
        auto child = transform->GetChild(childIndex)->GetObject();
        child->GetAllChildren(children);
        children.push_back(child);
    }
}

void Object::Destroy(float delay)
{
    if(timeToDestruction >= delay)
        timeToDestruction = delay;
    registeredForDestruction = true;
}

void Object::SetActive(bool active)
{
    selfActive = active;
}

bool Object::IsActive()
{
    return isActive;
}

bool Object::IsSelfActive()
{
    return selfActive;
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
        for (int i = 0; i < scripts.size(); i++)
        {
            auto script = scripts[i].get();
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

    return isSomeComponentDestroyed || (isActiveDirty && !isActive);
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
    std::erase_if(audios, [](const std::unique_ptr<Audio>& audio)
    {
        return audio->ToBeDestroyed();
    });
    if (collider && collider->ToBeDestroyed())
        collider.reset();
    std::erase_if(scripts, [](const std::unique_ptr<Script>& script)
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

void Object::UpdateActivationInChildren()
{
    for (int childIndex = 0; childIndex < transform->GetChildCount(); childIndex++)
    {
        auto childObject = transform->GetChild(childIndex)->GetObject();

        if (!childObject->isActive && childObject->selfActive && isActive)
        {
            childObject->isActive = true;
            childObject->isActiveDirty = true;

            for (int i = 0; i < childObject->scripts.size(); i++)
                childObject->scripts[i]->OnEnable();

            components.AddComponentsOfObject(childObject);
        }
        else if (childObject->isActive && (!childObject->selfActive || !isActive))
        {
            childObject->isActive = false;
            childObject->isActiveDirty = true;

            for (int i = 0; i < childObject->scripts.size(); i++)
                childObject->scripts[i]->OnDisable();
        }
        else
            childObject->isActiveDirty = false;

        childObject->UpdateActivationInChildren();
    }
}
