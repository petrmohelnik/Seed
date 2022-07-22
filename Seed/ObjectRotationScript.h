#pragma once
#include "Script.h"

class ObjectRotationScript : public Script
{
private:
    inline ObjectRotationScript* Clone() { return new ObjectRotationScript(*this); }
public:
    using Script::Script;

    void OnMouseDrag() override;

    float RotationSensitivity = 0.003f;
};

void ObjectRotationScript::OnMouseDrag()
{
    transform->RotateY(input.MouseDeltaPosition().x * RotationSensitivity, Transform::Space::World);
    transform->RotateX(input.MouseDeltaPosition().y * RotationSensitivity, Transform::Space::World);
}