#pragma once
#include "Script.h"

class CameraMovementScript : public Script
{
public:
    using Script::Script;

    void Update() override;

    float RotationSensitivity = 0.01;
    float MoveSensitivity = 0.1;
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
        //rotate around y global axis
        //transform->SetLocalRotation(glm::normalize(transform->GetLocalRotation() * glm::angleAxis(input.MouseMovement().x * RotationSensitivity, glm::vec3(0.0f, 1.0f, 0.0f))));
        //rotate around x local axis
        //transform->RotateX(input.MouseMovement().y * RotationSensitivity);
        //first around x, then around y
        transform->SetLocalRotation(glm::normalize(
            glm::angleAxis(-input.MouseMovement().x * RotationSensitivity, glm::vec3(0.0f, 1.0f, 0.0f)) *
            transform->GetLocalRotation() *
            glm::angleAxis(-input.MouseMovement().y * RotationSensitivity, glm::vec3(1.0f, 0.0f, 0.0f))));
    }
}
