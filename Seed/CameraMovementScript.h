#pragma once
#include "Script.h"

class CameraMovementScript : public Script
{
public:
    using Script::Script;

    void Update() override;

    float RotationSensitivity = 0.01f;
    float MoveSensitivity = 0.1f;
};

void CameraMovementScript::Update()
{
    if (input.Key(SDLK_w))
        transform->Translate(-transform->GetForwardAxis() * MoveSensitivity);
    if (input.Key(SDLK_s))
        transform->Translate(transform->GetForwardAxis() * MoveSensitivity);
    if (input.Key(SDLK_d))
        transform->Translate(transform->GetRightAxis() * MoveSensitivity);
    if (input.Key(SDLK_a))
        transform->Translate(-transform->GetRightAxis() * MoveSensitivity);

    if (input.MouseButton(SDL_BUTTON_RIGHT))
    {
        transform->RotateY(-input.MouseMovement().x * RotationSensitivity, Transform::Space::World);
        transform->RotateX(-input.MouseMovement().y * RotationSensitivity);
    }
}
