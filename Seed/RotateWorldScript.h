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
		transform->RotateAround(-input.MouseMovement().x * 0.01, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));
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