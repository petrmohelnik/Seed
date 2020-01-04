#pragma once
#include "Script.h"

class PlayerScript : public Script
{
public:
    using Script::Script;

    void OnCreate() override;
    void Update() override;

    float RotationSensitivity = 0.5f;
    float MoveSensitivity = 10.0f;

    Light* flashLight;
    Camera* camera;
};

void PlayerScript::OnCreate()
{
    camera = objects.GetObjectByName("PlayerCamera")->GetComponent<Camera>();
    flashLight = objects.GetObjectByName("FlashLight")->GetComponent<Light>();
}

void PlayerScript::Update()
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
        camera->GetTransform()->RotateY(-input.MouseMovement().x * RotationSensitivity * time.DeltaTime(), Transform::Space::World);
        camera->GetTransform()->RotateX(-input.MouseMovement().y * RotationSensitivity * time.DeltaTime());
    }

    if (input.KeyDown(SDLK_f))
    {
        if (flashLight->GetIntensity() != 0.0)
            flashLight->SetIntensity(0.0);
        else
            flashLight->SetIntensity(100.0);
    }
}
