#pragma once
#include "CameraObject.h"

void DefaultScene(Objects& objects, FileSystem& fileSystem)
{
    auto cameraObject = objects.CreateObject<CameraObject>("camera");
    auto cameraObject1 = objects.CreateObject<CameraObject>("camera1");
    auto cameraObject2 = objects.CreateObject<CameraObject>("camera2");
    auto cameraObject3 = objects.CreateObject<CameraObject>("camera3");
    auto cameraObject4 = objects.CreateObject<CameraObject>("camera4");
    cameraObject1->GetComponent<Transform>()->SetParent(cameraObject);
    cameraObject2->GetComponent<Transform>()->SetParent(cameraObject->GetComponent<Transform>());
    cameraObject3->GetComponent<Transform>()->SetParent(cameraObject2->GetComponent<Transform>());
    cameraObject4->GetComponent<Transform>()->SetParent(cameraObject2->GetComponent<Transform>());
    cameraObject->AddTag("tag");
    cameraObject->AddComponent<Light>()->Destroy(16000);;
    cameraObject4->AddComponent<Camera>();
    cameraObject->AddComponent<MeshRenderer>();
    cameraObject4->AddComponent<CameraScript>();
    cameraObject2->AddComponent<Audio>();
    cameraObject3->AddComponent<Rigidbody>();
    cameraObject->AddComponent<Collider>();
    cameraObject->AddComponent<Audio>();
    cameraObject->GetComponent<MeshRenderer>();
    cameraObject->GetComponent<Light>();
    cameraObject4->GetComponent<Camera>()->Destroy();
    cameraObject->GetComponent<Transform>();
    cameraObject->GetComponent<Audio>();
    cameraObject->GetComponent<Collider>();
    cameraObject3->GetComponent<Rigidbody>()->Destroy(15000);
    cameraObject2->Destroy(5000);
    objects.GetObjectsByName("camera");
    objects.GetObjects<CameraObject>();
    objects.GetObjectsByTag("tag");
    
    cameraObject->GetComponent<MeshRenderer>()->GetMesh() = fileSystem.LoadMesh("lara.dae");
}
