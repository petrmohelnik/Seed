#include "Object.h"
#include "Transform.h"
#include "Renderer.h"
#include "Audio.h"
#include "Script.h"

Object::Object(std::string name, Objects& objects, Components& components)
    : name(std::move(name)), objects(objects), components(components)
{
	transform = std::make_shared<Transform>(weak_from_this());
    id = objects.CreateUniqueId();
}

Objects::UniqueId Object::GetUniqueId()
{
    return id;
}

std::string Object::GetName()
{
    return name;
}

void Object::AddTag(const std::string& tag)
{
    tags.insert(tag);
}

bool Object::ContainsTag(const std::string & tag)
{
    return tags.count(name) != 0;
}

void Object::Destroy()
{
    objects.RegisterForDestruction(id);
}

void Object::Initialize()
{
}

void Object::RegisterForComponentDestruction()
{
    objects.RegisterForComponentDestruction(id);
}

void Object::DestroyComponents()
{
    if(renderer->IsRegisteredForDestruction())
        renderer.reset();
    if (camera->IsRegisteredForDestruction())
        camera.reset();
    if (light->IsRegisteredForDestruction())
        light.reset();
    std::experimental::erase_if(audios, [](const std::shared_ptr<Audio>& audio)
    {
        return audio->IsRegisteredForDestruction();
    });
    std::experimental::erase_if(colliders, [](const std::shared_ptr<Collider>& collider)
    {
        return collider->IsRegisteredForDestruction();
    });
    if (rigidbody->IsRegisteredForDestruction())
        rigidbody.reset();
    std::experimental::erase_if(scripts, [](const std::shared_ptr<Script>& script)
    {
        return script->IsRegisteredForDestruction();
    });
}
