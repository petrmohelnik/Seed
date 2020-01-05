#pragma once
#include "Object.h"
#include "PlayerScript.h"
#include "Engine.h"

class PlayerObject : public Object
{
public:
    using Object::Object;

    void Initialize() override;
};

void PlayerObject::Initialize()
{
    AddComponent<CapsuleCollider>()->SetCapsule(glm::vec3(0.0f, 0.9f, 0.0f), 1.8f, 0.4f);
    AddComponent<Rigidbody>()->SetIsKinematic(true);
    
    auto camera = Engine::GetObjects().CreateObject("PlayerCamera");
    camera->AddComponent<Camera>();
    camera->GetComponent<Transform>()->SetPosition(glm::vec3(0.0f, 1.7f, 0.0f));
    camera->GetComponent<Transform>()->SetParent(this);

    auto flashLight = Engine::GetObjects().CreateObject("FlashLight");
    flashLight->AddComponent<Light>();
    flashLight->GetComponent<Light>()->SetColor(glm::vec3(1.0f, 1.0f, 0.9f));
    flashLight->GetComponent<Light>()->SetSpotLight(glm::vec3(-0.1f, 0.15f, -1.0f), 30.0f, 100.0f, 0.5f, 0.4f);
    flashLight->GetComponent<Transform>()->SetPosition(glm::vec3(0.2f, -0.5f, 0.2f));
    flashLight->GetComponent<Transform>()->SetParent(camera);
    
    AddComponent<PlayerScript>();
}