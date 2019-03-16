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
	if (input.MouseButton(SDL_BUTTON_LEFT))
		transform->RotateY(-input.MouseMovement().x * 0.01, Transform::Space::World);
}