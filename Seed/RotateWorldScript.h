#pragma once
#include "Script.h"

class RotateWorldScript : public Script
{
public:
	using Script::Script;

	void Update() override;
};

void RotateWorldScript::Update()
{
	if (input.MouseButton(SDL_BUTTON_MIDDLE))
		transform->Rotate(glm::vec3(input.MouseMovement().y * 0.01f, input.MouseMovement().x * 0.01f, 0.0f), Transform::Space::World);
    if (input.KeyDown(SDLK_x))
    {
        transform->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    }
    if (input.KeyDown(SDLK_c))
    {
        transform->Translate(transform->GetRightAxis(), Transform::Space::World);
    }
    if (input.KeyDown(SDLK_v))
    {
        transform->Translate(transform->GetUpAxis(), Transform::Space::World);
    }

}