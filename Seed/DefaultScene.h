#pragma once
#include "Objects.h"
#include "Components.h"
#include "CameraObject.h"

void DefaultScene(Objects& objects, Components& components)
{
    auto cameraObject = objects.CreateObject<CameraObject>();
    auto mat = cameraObject->GetComponent<CameraScript>()->getProjection();
    if (mat != glm::mat4(0.0))
        throw std::exception("fail");
}
