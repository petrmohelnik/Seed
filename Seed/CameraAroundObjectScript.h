#pragma once
#include "Script.h"

class CameraAroundObjectScript : public Script
{
private:
    inline CameraAroundObjectScript* Clone() { return new CameraAroundObjectScript(*this); }
public:
    using Script::Script;

    void Update() override;

    float RotationSensitivity = 0.003f;
    float ScrollSensitivity = 0.1f;
};

void CameraAroundObjectScript::Update()
{
    if (input.MouseButton(SDL_BUTTON_RIGHT))
    {
        //transform->RotateX(-input.MouseDeltaPosition().y * RotationSensitivity, Transform::Space::World);
        //transform->RotateZ(-input.MouseDeltaPosition().x * RotationSensitivity, Transform::Space::World);
        transform->RotateAround(-input.MouseDeltaPosition().y * RotationSensitivity, transform->GetRightAxis());
        transform->RotateAround(-input.MouseDeltaPosition().x * RotationSensitivity, transform->GetUpAxis());
    }
    transform->Translate(input.MouseDeltaWheelPosition().y * ScrollSensitivity * transform->GetForwardAxis());
    transform->LookAt(glm::vec3(0.0f));
}