#pragma once
#include "CameraObject.h"

void DefaultScene(Objects& objects)
{
    auto cameraObject = objects.CreateObject<CameraObject>();
    cameraObject->AddLight();
    cameraObject->AddCamera();
    cameraObject->AddRenderer<MeshRenderer>();
    cameraObject->AddScript<CameraScript>();
    cameraObject->AddAudio();
    cameraObject->AddCollider<Collider>();
    cameraObject->AddAudio();
    cameraObject->GetComponent<MeshRenderer>();
    cameraObject->GetComponent<Light>();
    cameraObject->GetComponent<Camera>();
    cameraObject->GetComponent<Transform>();
    cameraObject->GetComponent<Audio>();
    cameraObject->GetComponent<Collider>();
    cameraObject->GetComponent<Rigidbody>();
    auto mat = cameraObject->GetComponent<CameraScript>()->getProjection();
    if (mat != glm::mat4(0.0))
        throw std::exception("fail");
}
