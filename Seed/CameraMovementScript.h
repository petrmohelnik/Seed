#pragma once
#include "Script.h"

class CameraMovementScript : public Script
{
public:
    using Script::Script;

    void OnCreate() override;
    void Update() override;

    float RotationSensitivity = 0.5f;
    float MoveSensitivity = 10.0f;

    Light* flashLight;
};

void CameraMovementScript::OnCreate()
{
    flashLight = objects.CreateObject("FlashLight")->AddComponent<Light>();
    flashLight->SetColor(glm::vec3(1.0f, 1.0f, 0.9f));
    flashLight->SetSpotLight(glm::vec3(-0.2f, 0.3f, -1.0f), 30.0f, 100.0f, 0.5f, 0.2f);
    flashLight->GetTransform()->SetPosition(glm::vec3(0.3f, -0.5f, 0.0f));
    flashLight->GetTransform()->SetParent(transform->GetObject());
}

void CameraMovementScript::Update()
{
    if (input.Key(SDLK_w))
        transform->Translate(-transform->GetForwardAxis() * MoveSensitivity * time.DeltaTime(), Transform::Space::World);
    if (input.Key(SDLK_s))
        transform->Translate(transform->GetForwardAxis() * MoveSensitivity * time.DeltaTime(), Transform::Space::World);
    if (input.Key(SDLK_d))
        transform->Translate(transform->GetRightAxis() * MoveSensitivity * time.DeltaTime(), Transform::Space::World);
    if (input.Key(SDLK_a))
        transform->Translate(-transform->GetRightAxis() * MoveSensitivity * time.DeltaTime(), Transform::Space::World);

    if (input.MouseButton(SDL_BUTTON_RIGHT))
    {
        transform->RotateY(-input.MouseMovement().x * RotationSensitivity * time.DeltaTime(), Transform::Space::World);
        transform->RotateX(-input.MouseMovement().y * RotationSensitivity * time.DeltaTime());
    }

    if (input.KeyDown(SDLK_f))
    {
        if (flashLight->GetIntensity() != 0.0)
            flashLight->SetIntensity(0.0);
        else
            flashLight->SetIntensity(100.0);
    }
}
