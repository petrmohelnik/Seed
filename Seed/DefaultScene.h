#pragma once
#include "CameraObject.h"

void DefaultScene(Objects& objects)
{
    auto cameraObject = objects.CreateObject<CameraObject>("camera");
    cameraObject->AddTag("tag");
    cameraObject->AddComponent<Light>();
    cameraObject->AddComponent<Camera>();
    cameraObject->AddComponent<MeshRenderer>();
    cameraObject->AddComponent<CameraScript>();
    cameraObject->AddComponent<Audio>();
    cameraObject->AddComponent<Rigidbody>();
    cameraObject->AddComponent<Collider>();
    cameraObject->AddComponent<Audio>();
    cameraObject->GetComponent<MeshRenderer>();
    cameraObject->GetComponent<Light>();
    cameraObject->GetComponent<Camera>()->Destroy();
    cameraObject->GetComponent<Transform>();
    cameraObject->GetComponent<Audio>();
    cameraObject->GetComponent<Collider>();
    cameraObject->GetComponent<Rigidbody>()->Destroy();
    cameraObject->Destroy();
    objects.GetObjectsByName("camera");
    objects.GetObjects<CameraObject>();
    objects.GetObjectsByTag("tag");
    auto mat = cameraObject->GetComponent<CameraScript>()->getProjection();
    if (mat != glm::mat4(0.0))
        throw std::exception("fail");
}
