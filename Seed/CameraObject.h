#pragma once
#include "Object.h"
#include "CameraMovementScript.h"

class CameraObject : public Object
{
public:
    using Object::Object;

    void Initialize() override;
};

void CameraObject::Initialize()
{
    AddComponent<CameraMovementScript>();
    AddComponent<Camera>();
}