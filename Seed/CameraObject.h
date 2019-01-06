#pragma once
#include "Object.h"
#include "CameraScript.h"

class CameraObject : public Object
{
public:
    using Object::Object;

    void Initialize() override;
};

void CameraObject::Initialize()
{
    AddScript(components.CreateScript<CameraScript>());
}