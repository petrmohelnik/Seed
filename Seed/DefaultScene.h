#pragma once
#include "CameraObject.h"

void DefaultScene(Objects& objects)
{
    auto cameraObject = objects.CreateObject<CameraObject>("camera");
    auto cameraObject1 = objects.CreateObject<CameraObject>("camera1");
    auto cameraObject2 = objects.CreateObject<CameraObject>("camera2");
    auto cameraObject3 = objects.CreateObject<CameraObject>("camera3");
    auto cameraObject4 = objects.CreateObject<CameraObject>("camera4");
    cameraObject1->GetComponent<Transform>()->SetParent(cameraObject->GetComponent<Transform>());
    cameraObject2->GetComponent<Transform>()->SetParent(cameraObject->GetComponent<Transform>());
    cameraObject3->GetComponent<Transform>()->SetParent(cameraObject2->GetComponent<Transform>());
    cameraObject4->GetComponent<Transform>()->SetParent(cameraObject2->GetComponent<Transform>());
    cameraObject->AddTag("tag");
    cameraObject->AddComponent<Light>();
    cameraObject4->AddComponent<Camera>();
    cameraObject2->AddComponent<MeshRenderer>();
    cameraObject4->AddComponent<CameraScript>();
    cameraObject2->AddComponent<Audio>();
    cameraObject3->AddComponent<Rigidbody>();
    cameraObject->AddComponent<Collider>();
    cameraObject->AddComponent<Audio>();
    cameraObject3->GetComponent<MeshRenderer>();
    cameraObject->GetComponent<Light>();
    cameraObject4->GetComponent<Camera>()->Destroy();
    cameraObject->GetComponent<Transform>();
    cameraObject->GetComponent<Audio>();
    cameraObject->GetComponent<Collider>();
    cameraObject3->GetComponent<Rigidbody>()->Destroy();
    cameraObject2->Destroy();
    objects.GetObjectsByName("camera");
    objects.GetObjects<CameraObject>();
    objects.GetObjectsByTag("tag");
    auto mat = cameraObject->GetComponent<CameraScript>()->getProjection();
    if (mat != glm::mat4(0.0))
        throw std::exception("fail");
}
