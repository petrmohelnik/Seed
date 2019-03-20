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
    glm::vec3 scale;
	if (input.MouseButton(SDL_BUTTON_LEFT))
		transform->RotateY(-input.MouseMovement().x * 0.01, Transform::Space::World);
    if (input.KeyDown(SDLK_x))
    {
        transform->Translate(transform->GetForwardAxis(), Transform::Space::World);
        scale = -transform->GetScale();
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