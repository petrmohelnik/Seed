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
    if (input.Key(SDLK_s))
    if (input.Key(SDLK_d))
    if (input.Key(SDLK_a))

    if (input.MouseButton(SDL_BUTTON_RIGHT))
    {
        //rotate around y global axis
        //rotate around x local axis
    }
}
