#pragma once
#include "Script.h"

class CameraMovementScript : public Script
{
public:
    using Script::Script;

    void Update() override;

    float RotationSensitivity = 0.5f;
    float MoveSensitivity = 10.0f;
};

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
}
