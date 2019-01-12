#include "Object.h"
#include "Transform.h"
#include "Renderer.h"
#include "Audio.h"
#include "Script.h"

Object::Object(Components& components) 
    : components(components)
{
	transform = std::make_shared<Transform>(weak_from_this());
}

void Object::Initialize()
{
}

Camera* Object::AddCamera()
{
    camera = components.CreateCamera(weak_from_this());
    return camera.get();
}

Light* Object::AddLight()
{
    light = components.CreateLight(weak_from_this());
    return light.get();
}

Audio* Object::AddAudio()
{
    auto audio = components.CreateAudio(weak_from_this());
    audios.push_back(audio);
    return audio.get();
}

Rigidbody* Object::AddRigidbody()
{
    rigidbody = components.CreateRigidbody(weak_from_this());
    return rigidbody.get();
}
